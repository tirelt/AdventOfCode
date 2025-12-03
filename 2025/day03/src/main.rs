use std::fs;
use std::collections::HashMap;

fn part_1(line: &str) -> i32 {
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


fn part_2(line: &str, current: i32,remaining: i32,memo: &mut HashMap<(i32,i32),String>) -> String{
    if let Some(v) = memo.get(&(remaining,remaining)){
        return v.clone();
    }
    if line.len() as i32 - current == remaining {
        return line.to_string();
    }
    String::new()
}
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut res_1 = 0;
    for line in file.lines(){
        let max = part_1(&line); // line.len()>1 ininput
        res_1 += max;
    }
    println!("Part 1: {res_1}")
}
