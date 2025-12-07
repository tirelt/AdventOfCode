use std::collections::BTreeMap;
use std::collections::BTreeSet;
use std::fs;

fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut map = Vec::new();
    let mut beams = BTreeSet::new();
    let mut beams_map = BTreeMap::new();
    let mut j = 0;
    for line in file.lines() {
        let mut v = Vec::new();
        for c in line.chars() {
            if c == 'S' {
                beams.insert(j);
                beams_map.insert(j, 1 as u128);
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
    for i in 0..(map.len() - 1) {
        let mut new_beams = BTreeMap::new();
        for j in beams_map {
            if map[i + 1][j.0] == '^' {
                *new_beams.entry(j.0 - 1).or_insert(0) += j.1;
                *new_beams.entry(j.0 + 1).or_insert(0) += j.1;
            } else {
                *new_beams.entry(j.0).or_insert(0) += j.1;
            }
        }
        beams_map = new_beams;
    }
    let mut res_2 = 0;
    for j in beams_map {
        res_2 += j.1;
    }
    println!("Part 2: {res_2}");
}
