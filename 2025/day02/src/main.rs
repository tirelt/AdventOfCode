use std::fs;

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
    let mut max = 0;
    let mut sum= 0;
    let mut res_1: u128 = 0;
    for range in file.split(',') {
        let mut ite = range.split('-');
        let start_str = ite.next().unwrap();
        let start: u128= start_str.parse().unwrap();
        let end_str = ite.next().unwrap();
        let end: u128 = end_str.parse().unwrap();
        let size = end - start;
        if end_str.len()%2 == 1 && start_str.len()%2==1{
            continue;
        }
        sum += size;
        max = std::cmp::max(max,size);
        for i in start..=end {
            if repeated_twice(i) {
                res_1 += i;
            }
        }
    }
    println!("Part 1: {res_1}");
    
}
