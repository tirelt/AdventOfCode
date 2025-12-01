use std::fs;

fn part_1(file_string: &String) {
    let mut pos = 50;
    let mut res = 0;
    for line in file_string.lines(){
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

fn part_2(file_string: &String) {
    let mut pos = 50;
    let mut res = 0;
    let mut new_pos;
    for line in file_string.lines(){
        let instructions = line.trim();
        let mut ite = instructions.chars();
        let dir = ite.next().unwrap();
        let rest: String = ite.collect();
        let mut n: i32 = rest.parse().unwrap();
        res += n/100; 
        n = n % 100;
        if n != 0 {
            if dir == 'L' { 
                new_pos = pos - n;
                if new_pos <= 0 && pos != 0{
                    res += 1;
                }
                if new_pos < 0 {
                    new_pos += 100;
                }
            } else {
                res += (pos + n ) / 100;
                new_pos = (pos + n ) % 100;
            }
            pos = new_pos;
        }
    }
    println!("Part 2: {res}");

}
fn main() {
    let file = fs::read_to_string("input").expect("should work");
    part_1(&file);
    part_2(&file);
}
