use std::fs;

fn max_string(line: &str) -> i32 {
    let mut ite = line.chars().peekable();
    let mut max = ite.next().unwrap();
    let mut ite_max = ite.clone();
    while let Some(c) = ite.next() {
        if let Some(&_n) = ite.peek() && max < c {
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
    let res = format!("{}{}",max,second_max);
    res.parse().unwrap()
}
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut res_1 = 0;
    for line in file.lines(){
        let max = max_string(&line); // line.len()>1 ininput
        res_1 += max;
    }
    println!("Part 1: {res_1}")
}
