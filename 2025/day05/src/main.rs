use std::collections::BTreeMap;
use std::fs;
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut items_to_check: Vec<i128> = Vec::new();
    let mut ranges: BTreeMap<i128, i32> = BTreeMap::new();
    let mut seen_empty = false;
    for line in file.lines() {
        if line.len() > 0 {
            if seen_empty {
                items_to_check.push(line.parse().unwrap());
            } else {
                let mut num_ite = line.split('-');
                let start: i128 = num_ite.next().unwrap().parse().unwrap();
                let end: i128 = num_ite.next().unwrap().parse().unwrap();
                let range_ite = ranges.entry(start).or_insert(0);
                *range_ite += 1;
                let range_ite = ranges.entry(end + 1).or_insert(0);
                *range_ite -= 1;
            }
        } else {
            seen_empty = true;
        }
    }
    items_to_check.sort();
    let mut ranges_ite = ranges.iter();
    let mut front_range = ranges_ite.next().unwrap();
    let mut range_count = 0;
    let mut no_more_ranges = false;
    let mut res_1 = 0;
    for item in items_to_check.iter() {
        while !no_more_ranges && item >= front_range.0 {
            range_count += front_range.1;
            if let Some(next_range) = ranges_ite.next() {
                front_range = next_range;
            } else {
                no_more_ranges = true;
            }
        }
        if range_count > 0 {
            res_1 += 1;
        }
    }
    println!("Part 1: {res_1}");
}
