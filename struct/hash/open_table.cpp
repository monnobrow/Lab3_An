#include "open_table.h"
#include "utils.h"
#include <stdexcept>
using namespace std;

const string OPEN_EMPTY = "empty";
const string OPEN_BUSY = "busy";
const string OPEN_DELETED = "deleted";

OpenTable::OpenTable(int mesto, bool double_hash, bool folding) {
  this->double_hash = double_hash;
  this->folding = folding;
  OCreate(mesto);
}

OpenTable::~OpenTable() {
  delete keys;
  delete values;
  delete states;
}

bool OpenTable::IsPrime(int value) const {
  if (value < 2)
    return false;
  for (int i = 2; i <= value / i; ++i) {
    if (value % i == 0)
      return false;
  }
  return true;
}

int OpenTable::NextPrime(int value) const {
  if (value < 3)
    value = 3;
  while (!IsPrime(value))
    ++value;
  return value;
}

// полиномиальный хеш, как в таблице с цепочками
int OpenTable::HHash(const string &key) const {
  unsigned int hash = 0;
  for (unsigned char c : key) {
    hash = hash * 31 + c;
  }
  return hash % mesto;
}

// метод свёртки: режем ключ по 3 цифры и складываем
int OpenTable::FoldHash(const string &key) const {
  unsigned int result = 0;
  int len = key.size();
  for (int i = 0; i < len; i += 3) {
    int part = 0;
    for (int j = i; j < i + 3 && j < len; ++j) {
      if (key[j] < '0' || key[j] > '9')
        return HHash(key); // не число - обычный хеш
      part = part * 10 + key[j] - '0';
    }
    result = (result + part) % mesto;
  }
  return result;
}

int OpenTable::OpenStart(const string &key) const {
  if (folding)
    return FoldHash(key);
  return HHash(key);
}

// шаг пробирования: 1 - линейное, вторая хеш-функция (FNV) - двойное
int OpenTable::OpenStep(const string &key) const {
  if (!double_hash)
    return 1;
  unsigned int hash = 2166136261u;
  for (unsigned char c : key) {
    hash ^= c;
    hash *= 16777619u;
  }
  return 1 + hash % (mesto - 1);
}

// три массива по mesto пустых ячеек
void OpenTable::OCreate(int new_mesto) {
  mesto = NextPrime(new_mesto);
  keys = new Array(mesto);
  values = new Array(mesto);
  states = new Array(mesto);
  for (int i = 0; i < mesto; ++i) {
    keys->MAddEnd("");
    values->MAddEnd("");
    states->MAddEnd(OPEN_EMPTY);
  }
  size = 0;
  deleted = 0;
}

int OpenTable::OpenFind(const string &key) const {
  int index = OpenStart(key);
  int step = OpenStep(key);
  for (int i = 0; i < mesto; ++i) {
    string state = states->MGet(index);
    if (state == OPEN_EMPTY)
      return -1; // пустая ячейка обрывает поиск
    if (state == OPEN_BUSY && keys->MGet(index) == key)
      return index;
    index = (index + step) % mesto;
  }
  return -1;
}

// кладём в первую незанятую ячейку по пути пробирования
void OpenTable::OPlace(const string &key, const string &value) {
  int index = OpenStart(key);
  int step = OpenStep(key);
  while (states->MGet(index) == OPEN_BUSY) {
    index = (index + step) % mesto;
  }
  if (states->MGet(index) == OPEN_DELETED)
    --deleted;
  keys->MReplace(key, index);
  values->MReplace(value, index);
  states->MReplace(OPEN_BUSY, index);
  ++size;
}

void OpenTable::OpenRehash(int new_mesto) {
  Array *old_keys = keys;
  Array *old_values = values;
  Array *old_states = states;
  int old_mesto = mesto;

  OCreate(new_mesto); // новые пустые массивы
  for (int i = 0; i < old_mesto; ++i) {
    if (old_states->MGet(i) == OPEN_BUSY) {
      OPlace(old_keys->MGet(i), old_values->MGet(i));
    }
  }
  delete old_keys;
  delete old_values;
  delete old_states;
}

// true - новый ключ, false - обновили значение
bool OpenTable::OInsert(const string &key, const string &value) {
  int existing = OpenFind(key);
  if (existing >= 0) {
    values->MReplace(value, existing);
    return false;
  }
  // заполнено ~75% вместе с метками "deleted" - перестраиваем
  if (size + deleted + 1 >= mesto - mesto / 4) {
    int new_mesto = mesto; // в основном метки - тот же размер, просто чистим их
    if (size + 1 >= mesto / 2) {
      new_mesto = mesto * 2;
    }
    OpenRehash(new_mesto);
  }
  OPlace(key, value);
  return true;
}

string OpenTable::OGet(const string &key) const {
  int index = OpenFind(key);
  if (index < 0) {
    throw runtime_error("ключ " + key + " не найден");
  }
  return values->MGet(index);
}

bool OpenTable::OContains(const string &key) const {
  return OpenFind(key) >= 0;
}

// не очищаем до "empty", а ставим метку, иначе оборвём поиск других ключей
void OpenTable::ODelete(const string &key) {
  int index = OpenFind(key);
  if (index < 0) {
    throw runtime_error("ключ " + key + " не найден");
  }
  keys->MReplace("", index);
  values->MReplace("", index);
  states->MReplace(OPEN_DELETED, index);
  --size;
  ++deleted;
}

int OpenTable::OLength() const { return size; }

void OpenTable::OPrint() const {
  for (int i = 0; i < mesto; ++i) {
    cout << '[' << i << "] ";
    string state = states->MGet(i);
    if (state == OPEN_BUSY) {
      cout << keys->MGet(i) << " -> " << values->MGet(i);
    } else if (state == OPEN_DELETED) {
      cout << "удалено";
    } else {
      cout << '-';
    }
    cout << '\n';
  }
}

void OpenTable::Save(const string &filename) const {
  StateWriter out(filename, 'O');
  out.number(size);
  for (int i = 0; i < mesto; ++i) {
    if (states->MGet(i) == OPEN_BUSY) {
      out.text(keys->MGet(i));
      out.text(values->MGet(i));
    }
  }
  out.finish();
}

void OpenTable::Load(const string &filename) {
  StateReader in(filename, 'O');
  int count;
  if (!in.count(count))
    return;
  std::vector<std::pair<std::string, std::string>> pairs;
  for (int i = 0; i < count; ++i) {
    auto key = in.text();
    pairs.emplace_back(std::move(key), in.text());
  }
  OCreate(11);
  for (const auto &entry : pairs)
    OInsert(entry.first, entry.second);
}
