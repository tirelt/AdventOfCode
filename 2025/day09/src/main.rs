use plotly::layout::Shape;
use plotly::{Layout, Plot, Scatter};
use std::cmp::{max, min};
use std::fs;
struct Tile {
    x: i64,
    y: i64,
}
impl Tile {
    fn area(&self, other: &Self) -> i64 {
        (self.x - other.x + 1).abs() * (self.y - other.y + 1).abs()
    }
}
fn rect_intersert_polygon(
    x_min: i64,
    y_min: i64,
    x_max: i64,
    y_max: i64,
    horizontal_edges: &Vec<(i64, i64, i64)>,
    vertical_edges: &Vec<(i64, i64, i64)>,
) -> bool {
    for (x, y_min_edge, y_max_edge) in vertical_edges {
        if *x > x_min && *x < x_max && max(y_min, *y_min_edge) < min(y_max, *y_max_edge) {
            return true;
        }
    }
    for (y, x_min_edge, x_max_edge) in horizontal_edges {
        if *y > y_min && *y < y_max && max(x_min, *x_min_edge) < min(x_max, *x_max_edge) {
            return true;
        }
    }
    false
}
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut tiles = Vec::new();
    let mut x_list = Vec::new();
    let mut y_list = Vec::new();
    for line in file.lines() {
        let mut ite = line.split(',');
        let x: i64 = ite.next().unwrap().parse().unwrap();
        let y: i64 = ite.next().unwrap().parse().unwrap();
        x_list.push(x);
        y_list.push(y);
        tiles.push(Tile { x, y });
    }

    let mut areas = Vec::new();
    let mut res_1 = 0;
    for i in 0..tiles.len() {
        for j in (i + 1)..tiles.len() {
            let d = tiles[i].area(&tiles[j]);
            if d > res_1 {
                res_1 = d;
            }
            areas.push((d, (i, j)));
        }
    }
    areas.sort_by(|x, y| y.0.cmp(&x.0));
    //Plotting
    let trace = Scatter::new(x_list, y_list).mode(plotly::common::Mode::LinesMarkers);
    let mut plot = Plot::new();
    plot.add_trace(trace);
    //Part 2
    let mut horizontal_edges = Vec::new();
    let mut vertical_edges = Vec::new();
    for i in 0..tiles.len() {
        let t1 = &tiles[i];
        let t2 = &tiles[(i + 1) % tiles.len()];
        if t1.x == t2.x {
            vertical_edges.push((t1.x, min(t1.y, t2.y), max(t1.y, t2.y)));
        } else {
            horizontal_edges.push((t1.y, min(t1.x, t2.x), max(t1.x, t2.x)));
        }
    }

    let mut res_2 = 0;
    let mut x0 = 0;
    let mut x1 = 0;
    let mut y0 = 0;
    let mut y1 = 0;

    for (area, (i, j)) in areas {
        let c1 = &tiles[i];
        let c2 = &tiles[j];
        let (x_min, y_min, x_max, y_max) = (
            min(c1.x, c2.x),
            min(c1.y, c2.y),
            max(c1.x, c2.x),
            max(c1.y, c2.y),
        );
        if !rect_intersert_polygon(
            x_min,
            y_min,
            x_max,
            y_max,
            &horizontal_edges,
            &vertical_edges,
        ) {
            res_2 = area;
            x0 = x_min;
            x1 = x_max;
            y0 = y_min;
            y1 = y_max;
            break;
        }
    }
    let rect = Shape::new()
        .x0(x0)
        .y0(y0)
        .x1(x1)
        .y1(y1)
        .shape_type(plotly::layout::ShapeType::Rect);
    let mut layout = Layout::new().width(900).height(900);
    layout.add_shape(rect);
    plot.set_layout(layout);
    plot.show();
    println!("Part 1: {res_1}");
    println!("Part 2: {res_2}");
}
