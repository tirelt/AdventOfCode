use good_lp::{
    Expression, ProblemVariables, Solution, SolverModel, solvers::highs::highs, variable,
};
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
        }
        let mut current_str = current_str.to_string();
        current_str.retain(|c| c != '{' && c != '}');
        for num_str in current_str.split(',') {
            let num: i32 = num_str.parse().unwrap();
            joltage.push(num);
        }
        assert_eq!(joltage.len(), buttons_2[0].len());

        let n_buttons = buttons.len();
        // Part 2
        let mut vars = ProblemVariables::new();
        let x: Vec<_> = (0..n_buttons)
            .map(|_| vars.add(variable().min(0).integer())) // integer, non-negative
            .collect();
        let obj: Expression = x.iter().cloned().sum();
        let mut problem = vars.minimise(obj.clone()).using(highs);
        for i in 0..joltage.len() {
            let mut expr: Expression = 0.0.into();
            for j in 0..n_buttons {
                if buttons_2[j][i] > 0 {
                    expr = expr + buttons_2[j][i] * x[j];
                }
            }
            problem = problem.with(expr.eq(joltage[i]));
        }
        let solution = problem.solve().unwrap();
        let values: Vec<f64> = x.into_iter().map(|x| solution.value(x)).collect();
        let min_val: f64 = solution.eval(obj);
        res_2 += min_val;

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
