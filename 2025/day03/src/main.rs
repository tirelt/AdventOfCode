use std::fs;

fn part_1_2(line: &Vec<char>, p: usize, n: usize) -> String {
    if n == 0 || p >= line.len() {
        return String::new();
    }
    let (m, new_p) = max_string(line, p, n - 1);
    let rest = part_1_2(line, new_p + 1, n - 1);
    format!("{}{}", m, rest)
}

fn max_string(v: &Vec<char>, p: usize, n: usize) -> (char, usize) {
    let mut max_p: usize = p;
    let mut m: char = v[p];
    for i in p..(v.len() - n) {
        if v[i] > m {
            m = v[i];
            max_p = i;
        }
    }
    (m, max_p)
}

fn string_to_array(line: &str) -> Vec<char> {
    let mut v = Vec::new();
    for c in line.chars() {
        v.push(c);
    }
    v
}
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut res_1 = 0;
    let mut res_2 = 0;
    for line in file.lines() {
        let v = string_to_array(&line);

        let max_1: String = part_1_2(&v, 0, 2);
        let max_1: u128 = max_1.parse().unwrap();
        res_1 += max_1;

        let max_2: String = part_1_2(&v, 0, 12);
        let max_2: u128 = max_2.parse().unwrap();
        res_2 += max_2;
    }
    println!("Part 1: {res_1}");
    println!("Part 2: {res_2}");
}
