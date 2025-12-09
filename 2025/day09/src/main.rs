use plotly::layout::Shape;
use plotly::{Layout, Plot, Scatter};
use std::collections::BTreeSet;
use std::collections::HashMap;
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
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let threshold = 50000; // data dependent
    let mut tiles = Vec::new();
    let mut counter = 0;
    let mut x_list = Vec::new();
    let mut y_list = Vec::new();
    let mut outlier = Vec::new();
    let mut prev_x = -1;
    let mut map_x = HashMap::new();
    let mut map_y = HashMap::new();
    for line in file.lines() {
        let mut ite = line.split(',');
        let x: i64 = ite.next().unwrap().parse().unwrap();
        let y: i64 = ite.next().unwrap().parse().unwrap();
        x_list.push(x);
        y_list.push(y);
        tiles.push(Tile { x, y });
        map_x.entry(x).or_insert(BTreeSet::new()).insert(counter);
        map_y.entry(y).or_insert(BTreeSet::new()).insert(counter);
        if prev_x > 0 {
            if (prev_x - x).abs() > threshold {
                outlier.push(counter);
            }
        }
        prev_x = x;
        counter += 1;
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
    let mut res_2 = 0;
    let mut outlier1 = &tiles[outlier[0]];
    let mut outlier2 = &tiles[outlier[1]];
    if outlier1.y > outlier2.y {
        outlier1 = &tiles[outlier[1]];
        outlier2 = &tiles[outlier[0]];
    }
    let (x0, x1, y0, y1) = (0, 0, 0, 0);
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
