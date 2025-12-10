use ndarray::Array;
use std::cmp::min;
use std::fs;

fn generate_combinations(n: usize) -> Vec<Vec<u8>> {
    let total = 1 << n;
    let mut result = Vec::with_capacity(total);

    for i in 0..total {
        let mut v = Vec::with_capacity(n);
        for j in (0..n).rev() {
            let bit = (i >> j) & 1;
            v.push(bit as u8);
        }
        result.push(v);
    }
    result
}
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut res_1 = 0;
    for line in file.lines() {
        let mut ite = line.split(" ");
        let mut objective = Vec::new();
        for c in ite.next().unwrap().chars() {
            match c {
                '#' => objective.push(1),
                '.' => objective.push(-1),
                _ => (),
            }
        }
        let objective = Array::from_vec(objective);
        let n_lights = objective.len();
        let mut buttons = Vec::new();
        'outer_loop: loop {
            let mut v = vec![1; n_lights];
            for c in ite.next().unwrap().chars() {
                if c == '{' {
                    break 'outer_loop;
                }
                if let Some(num) = c.to_digit(10) {
                    v[num as usize] = -1;
                }
            }
            buttons.push(Array::from_vec(v));
        }
        let n_buttons = buttons.len();
        let mut min_buttons = n_buttons;
        let all_combinations = generate_combinations(n_buttons as usize);
        for combination in all_combinations.iter() {
            let mut res = Array::from_vec(vec![-1; n_lights]);
            for i in 0..combination.len() {
                if combination[i] == 1 {
                    res = &res * &buttons[i];
                }
            }
            if res == objective {
                let sum: usize = combination.iter().map(|&x| x as usize).sum();
                min_buttons = min(min_buttons, sum);
            }
        }
        res_1 += min_buttons;
    }
    println!("Part 1: {res_1}");
}
