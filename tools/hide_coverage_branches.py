#!/usr/bin/env python3
"""Hide gcovr branch columns in generated HTML reports across gcovr versions."""

from pathlib import Path
import sys


STYLE = """
/* Keep the report focused on line and function coverage. */
.file-list colgroup:last-of-type,
.file-list tr > th[scope="colgroup"]:last-of-type,
.file-list td.branch-coverage,
.coverage tr:last-child,
.file-source tr > :nth-child(2) {
  display: none !important;
}
"""


def main() -> int:
    if len(sys.argv) != 2:
        print(f"Usage: {Path(sys.argv[0]).name} REPORT_DIRECTORY", file=sys.stderr)
        return 2

    report_dir = Path(sys.argv[1])
    pages = list(report_dir.glob("*.html"))
    if not pages:
        print(f"No HTML report pages found in {report_dir}", file=sys.stderr)
        return 1

    style_tag = f"<style>\n{STYLE}\n</style>\n"
    for page in pages:
        html = page.read_text(encoding="utf-8")
        if "</head>" not in html:
            raise ValueError(f"Missing </head> in {page}")
        if ".file-list colgroup:last-of-type" not in html:
            html = html.replace("</head>", style_tag + "</head>", 1)
            page.write_text(html, encoding="utf-8")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
