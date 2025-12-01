use std::fs;
fn main() {
    let file = fs::read_to_string("input").expect("should work");
    let mut pos = 50;
    let mut res = 0;
    for line in file.lines(){
        let instructions = line.trim();
        let mut ite = instructions.chars();
        let dir = ite.next().unwrap();
        let rest: String = ite.collect();
        let n: i32 = rest.parse().unwrap();
        if dir == 'L' { 
            pos = (pos + 100 - n) % 100;
        } else {
            pos = (pos + n ) % 100;
        }
        if pos == 0{
            res += 1;
        }
    }
    println!("Part 1: {res}");
}
