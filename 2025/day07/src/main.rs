use std::collections::BTreeSet;
use std::fs;

fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut map = Vec::new();
    let mut beams = BTreeSet::new();
    let mut j = 0;
    for line in file.lines() {
        let mut v = Vec::new();
        for c in line.chars() {
            if c == 'S' {
                beams.insert(j);
            }
            v.push(c);
            j += 1;
        }
        map.push(v);
        j = 0;
    }
    let mut res_1 = 0;
    for i in 0..(map.len() - 1) {
        let mut new_beams = BTreeSet::new();
        for j in beams {
            if map[i + 1][j] == '^' {
                new_beams.insert(j - 1);
                new_beams.insert(j + 1);
                res_1 += 1;
            } else {
                new_beams.insert(j);
            }
        }
        beams = new_beams;
    }
    println!("Part 1: {res_1}");
}
