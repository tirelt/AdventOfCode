from itertools import combinations
import argparse
from pathlib import Path

def is_point_inside(
    px: int, py: int, vertical_edges: list[tuple[int, int, int]]
) -> bool:
    """
    Raycasting to check if a point is inside a polygon.
    args:
        px: x coordinate of the point
        py: y coordinate of the point
        vertical_edges: list of vertical edges of the polygon
    returns:
        True if the point is inside the polygon, False otherwise
    """
    inside = False
    for edge_x, edge_ymin, edge_ymax in vertical_edges:
        if edge_x > px and (edge_ymin <= py < edge_ymax):
            inside = not inside
    return inside


def rectange_intersects_polygon(
    rectangle: tuple[int, int, int, int],
    vertical_edges: list[tuple[int, int, int]],
    horizontal_edges: list[tuple[int, int, int]],
) -> bool:
    """
    Check if a rectangle intersects a polygon. The rectangle intersects if at least one
    of the polygon edges is STRUCTLY inside the rectangle (i.e. not touching the edges).
    args:
        rectangle: tuple of (xmin, ymin, xmax, ymax) describing the rectangle
        vertical_edges: list of vertical edges of the polygon
        horizontal_edges: list of horizontal edges of the polygon
    returns:
        True if the rectangle intersects the polygon, False otherwise
    """
    xmin, ymin, xmax, ymax = rectangle
    for edge_x, edge_ymin, edge_ymax in vertical_edges:
        if xmin < edge_x < xmax and max(edge_ymin, ymin) < min(edge_ymax, ymax):
            return True

    for edge_xmin, edge_xmax, edge_y in horizontal_edges:
        if ymin < edge_y < ymax and max(edge_xmin, xmin) < min(edge_xmax, xmax):
            return True

    return False


def part1(data: str):
    corners = [list(map(int, line.split(","))) for line in data.splitlines()]
    best_so_far = 0
    for (x1, y1), (x2, y2) in combinations(corners, 2):
        best_so_far = max(best_so_far, (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1))
    return best_so_far


def part2(data: str):
    corners = [list(map(int, line.split(","))) for line in data.splitlines()]
    vertical_edges, horizontal_edges = [], []
    for i in range(len(corners)):
        (x1, y1), (x2, y2) = corners[i], corners[(i + 1) % len(corners)]
        if x1 == x2:
            vertical_edges.append((x1, min(y1, y2), max(y1, y2)))
        if y1 == y2:
            horizontal_edges.append((min(x1, x2), max(x1, x2), y1))

    best_so_far = 0
    for (x1, y1), (x2, y2) in combinations(corners, 2):
        area = (abs(x1 - x2) + 1) * (abs(y1 - y2) + 1)
        if area <= best_so_far:
            continue
        xmin, ymin, xmax, ymax = min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2)
        if is_point_inside(
            xmin + 0.1, ymin + 0.1, vertical_edges
        ) and not rectange_intersects_polygon(
            (xmin, ymin, xmax, ymax), vertical_edges, horizontal_edges
        ):
            best_so_far = area
    return best_so_far

def main():
    base_dir = Path(__file__).parent
    input_path = base_dir / f"input"
    data = input_path.read_text().strip()
    print(part2(data))

if __name__ == "__main__":
    main()