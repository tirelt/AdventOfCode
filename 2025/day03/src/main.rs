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


fn part_2(line: &Vec<char>, p: usize, n: usize) -> String{
    let last = line.len()-n;
    let m = line[p..last].iter().max().unwrap();
    let rest = part_2(line,new_p,n-1);
    format!("{}{}",m,rest)
}

fn max_string(v: &Vec<char>) -> (char,usize) {
    let mut p: usize = 0;
    let mut m: char = v[p];
    for i in 1..v.len(){
        if v[i] > m {
            m = v[i];
            p = i;
        }
    } 
    (m,p)
}

fn string_to_array(line: &str) -> Vec<char> {
    let mut v = Vec::new();
    for c in line.chars(){
        v.push(c);
    }
    v
}
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut res_1 = 0;
    for line in file.lines(){
        let v = string_to_array(&line);
        let max = part_1(&line); // line.len()>1 ininput
        res_1 += max;
    }
    println!("Part 1: {res_1}")
}
