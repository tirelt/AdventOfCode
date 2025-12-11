use minilp::{ComparisonOp, LinearExpr, OptimizationDirection, Problem};
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
    let mut res_2 = 0.0;
    for line in file.lines() {
        let mut problem = Problem::new(OptimizationDirection::Minimize);
        let mut variables = Vec::new();
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
        let mut buttons_2 = Vec::new();
        let mut joltage = Vec::new();
        let mut current_str;
        'outer_loop: loop {
            let mut v = vec![1; n_lights];
            let mut v_2 = vec![0; n_lights];
            current_str = ite.next().unwrap();
            for c in current_str.chars() {
                if c == '{' {
                    break 'outer_loop;
                }
                if let Some(num) = c.to_digit(10) {
                    v[num as usize] = -1;
                    v_2[num as usize] = 1;
                }
            }
            buttons.push(Array::from_vec(v));
            buttons_2.push(v_2);
            variables.push(problem.add_var(1.0, (0.0, f64::INFINITY)));
        }
        let mut current_str = current_str.to_string();
        current_str.retain(|c| c != '{' && c != '}');
        for num_str in current_str.split(',') {
            let num: i32 = num_str.parse().unwrap();
            joltage.push(num);
        }
        assert_eq!(joltage.len(), buttons_2[0].len());
        for i in 0..joltage.len() {
            let mut lhs = LinearExpr::empty();
            let mut j = 0;
            for &v in &variables {
                if buttons_2[j][i] > 0 {
                    lhs.add(v, buttons_2[j][i] as f64);
                }
                j += 1;
            }
            problem.add_constraint(lhs, ComparisonOp::Eq, joltage[i] as f64);
        }
        let solution = problem.solve().unwrap();
        let temp = solution.objective();
        if temp - (temp as i64 as f64) > 0.1 {
            let mut j = 0;
            let vars_solution: Vec<f64> = variables.into_iter().map(|x| solution[x]).collect();
            j += 1;
        }
        res_2 += temp;
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
    println!("Part 2: {res_2}");
}
