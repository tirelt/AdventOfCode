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
    let mut tiles = Vec::new();
    for line in file.lines() {
        let mut ite = line.split(',');
        let x: i64 = ite.next().unwrap().parse().unwrap();
        let y: i64 = ite.next().unwrap().parse().unwrap();
        tiles.push(Tile { x, y });
    }
    let mut res_1 = 0;
    for i in 0..tiles.len() {
        for j in (i + 1)..tiles.len() {
            let d = tiles[i].area(&tiles[j]);
            if d > res_1 {
                res_1 = d;
            }
        }
    }
    println!("Part 1: {res_1}");
}
