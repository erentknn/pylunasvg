import argparse
import sys

from . import PylunasvgError, svg2png


def main():
    parser = argparse.ArgumentParser(description="Convert SVG to PNG")
    parser.add_argument("svg", help="SVG file to convert")
    parser.add_argument("-W", "--width", default=None, type=int, help="Width of output")
    parser.add_argument("-H", "--height", default=None, type=int, help="Height of output")
    parser.add_argument("-s", "--scale", default=1.0, type=float, help="Scale of output")

    parser.add_argument(
        "-o",
        "--output",
        default=None,
        help="Output file name, default is input name with .png.",
    )

    args = parser.parse_args()

    try:
        svg2png(
            svg_file=args.svg,
            width=args.width,
            height=args.height,
            scale=args.scale,
            output_file=args.output,
        )
    except (PylunasvgError, ImportError) as e:
        print(f"error: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
