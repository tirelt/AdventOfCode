use std::collections::HashMap;
use std::fs;

fn part_1(line: &str) -> i32 {
    let mut ite = line.chars().peekable();
    let mut max = ite.next().unwrap();
    let mut ite_max = ite.clone();
    while let Some(c) = ite.next() {
        if let Some(&_n) = ite.peek()
            && max < c
        {
            max = c;
            ite_max = ite.clone();
        }
    }
    let mut second_max = '0';
    while let Some(c) = ite_max.next() {
        if second_max < c {
            second_max = c;
        }
    }
    let res = format!("{}{}", max, second_max);
    res.parse().unwrap()
}

fn part_1_2(line: &Vec<char>, p: usize, n: usize) -> String {
    if n == 0 || p >= line.len() {
        return String::new();
    }
    let (m, new_p) = max_string(line, p, n - 1);
    let rest = part_2(line, new_p + 1, n - 1);
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
        let max = part_1(&line); // line.len()>1 ininput
        let test_str: String = part_2(&v, 0, 12);
        let test: u128 = test_str.parse().unwrap();
        res_2 += test;
        res_1 += max;
    }
    println!("Part 1: {res_1}");
    println!("Part 2: {res_2}");
}
