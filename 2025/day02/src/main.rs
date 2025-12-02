use std::fs;

fn repeated_multiple(input: u128)-> bool {
    let i_str = input.to_string();
    let n = i_str.len();
    for i in 1..=n/2{
        if n % i != 0 {
            continue;
        }
        let first = &i_str[..i];
        let mut repeats = true;
        for j in 1..n/i{
            if first != &i_str[j*i..(j+1)*i]{
                repeats = false;
                break;
            }
        }
        if repeats {
            return true;
        }
    };
    false
}
fn repeated_twice(i: u128)-> bool {
    let i_str = i.to_string();
    let n = i_str.len();
    if n % 2 == 1 {
        return false;
    }
    i_str[..n/2] == i_str[n/2..]
}
fn main() {
    let file = fs::read_to_string("input").unwrap();
    let mut res_1: u128 = 0;
    let mut res_2: u128 = 0;
    for range in file.split(',') {
        let mut ite = range.split('-');
        let start_str = ite.next().unwrap();
        let start: u128= start_str.parse().unwrap();
        let end_str = ite.next().unwrap();
        let end: u128 = end_str.parse().unwrap();
        /* for Part 1
        if end_str.len()%2 == 1 && start_str.len()%2==1{
            continue;
        }*/
        for i in start..=end {
            if repeated_twice(i) {
                res_1 += i;
            }
            if repeated_multiple(i){
                res_2 += i;
            }
        }
    }
    println!("Part 1: {res_1}");
    println!("Part 2: {res_2}");
    
}
