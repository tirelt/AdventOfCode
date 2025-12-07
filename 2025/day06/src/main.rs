use std::fs;

fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut input_vec = Vec::new();
    for line in file.lines() {
        let mut v = Vec::new();
        let line = line.replace("  ", " ");
        for c in line.trim().split(' ') {
            if c.len() > 0 && c != " " {
                v.push(c.to_string());
            }
        }
        input_vec.push(v);
    }
    let n_lines = input_vec.len();
    let n_cols = input_vec[0].len();
    let mut res_1 = 0;
    for j in 0..n_cols {
        let op = &input_vec[n_lines - 1][j];
        let mut res = 0;
        if op == "*" {
            res = 1;
        }
        for i in 0..(n_lines - 1) {
            let operand: u128 = input_vec[i][j].parse().unwrap();
            if op == "*" {
                res *= operand;
            } else {
                res += operand;
            }
        }
        res_1 += res;
    }
    let mut input_vec_2 = Vec::new();
    let mut res_2 = 0;
    for line in file.lines() {
        let mut v = Vec::new();
        for c in line.chars() {
            v.push(c);
        }
        input_vec_2.push(v);
    }
    let mut v = Vec::new();
    'outer: for j in (0..input_vec_2[0].len()).rev() {
        let mut number = 0;
        for i in 0..input_vec_2.len() {
            let c = input_vec_2[i][j];
            if c == '*' || c == '+' {
                v.push(number);
                if c == '*' {
                    res_2 += v.iter().fold(1, |x, y| x * y);
                } else {
                    res_2 += v.iter().fold(0, |x, y| x + y);
                }
                v.clear();
                continue 'outer;
            }
            if c != ' ' {
                let temp: u128 = c.to_string().parse().unwrap();
                number = number * 10 + temp;
            }
        }
        if number != 0 {
            v.push(number);
        }
    }
    println!("Part 1: {res_1}");
    println!("Part 2: {res_2}");
}
