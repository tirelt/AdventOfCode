use std::fs;
use std::mem;
struct Boxe {
    x: i128,
    y: i128,
    z: i128,
    circuit: Option<usize>,
}

impl Boxe {
    fn dist(&self, other: &Self) -> f64 {
        (((self.x - other.x).pow(2) + (self.y - other.y).pow(2) + (self.z - other.z).pow(2)) as f64)
            .sqrt()
    }
    fn new(line: &str) -> Self {
        let mut ite = line.split(',');
        let x: i128 = ite.next().unwrap().parse().unwrap();
        let y: i128 = ite.next().unwrap().parse().unwrap();
        let z: i128 = ite.next().unwrap().parse().unwrap();
        Boxe {
            x,
            y,
            z,
            circuit: None,
        }
    }
}

struct Dist {
    boxe_1: usize,
    boxe_2: usize,
    dist: f64,
}

fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut boxes = Vec::new();
    for line in file.lines() {
        boxes.push(Boxe::new(line));
    }
    let n_boxes = boxes.len();
    let mut dists = Vec::new();
    for i in 0..n_boxes {
        for j in (i + 1)..n_boxes {
            let dist = boxes[i].dist(&boxes[j]);
            dists.push(Dist {
                boxe_1: i,
                boxe_2: j,
                dist: dist,
            })
        }
    }
    dists.sort_by(|x, y| x.dist.partial_cmp(&y.dist).unwrap());
    let mut counter = 0;
    let n_ite = 1000;
    let mut circuits: Vec<Vec<usize>> = Vec::new();
    for d in dists.iter() {
        if counter >= n_ite {
            break;
        }
        let c_1 = boxes[d.boxe_1].circuit;
        let c_2 = boxes[d.boxe_2].circuit;
        match (c_1, c_2) {
            (Some(c), None) => {
                boxes[d.boxe_2].circuit = Some(c);
                circuits[c].push(d.boxe_2);
            }
            (None, Some(c)) => {
                boxes[d.boxe_1].circuit = Some(c);
                circuits[c].push(d.boxe_1);
            }
            (None, None) => {
                let c = circuits.len();
                boxes[d.boxe_2].circuit = Some(c);
                boxes[d.boxe_1].circuit = Some(c);
                circuits.push(vec![d.boxe_1, d.boxe_2]);
            }
            (Some(i), Some(j)) => {
                if i != j {
                    let moved = mem::take(&mut circuits[j]);
                    for b in moved {
                        boxes[b].circuit = Some(i);
                        circuits[i].push(b);
                    }
                }
            }
        }
        counter += 1;
    }
    circuits.sort_by(|a, b| b.len().cmp(&a.len()));
    let res_1 = circuits[0].len() * circuits[1].len() * circuits[2].len();
    println!("Part 1: {res_1}");
}
