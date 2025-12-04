use std::fs;
fn main() {
    let file =fs::read_to_string("input").unwrap();
    let mut map = Vec::new();
    for line in file.lines(){
        let mut v = Vec::new();
        for c in line.chars() {
            v.push(c);
        }
        map.push(v);
    }
    let mut res_1 = 0;
    for i in 0..map.len() {
        for j in 0..map[i].len() {
            if map[i][j] != '@' {
                continue;
            }
            let i = i as i32;
            let j = j as i32;
            let pos_to_check = vec![(i-1,j-1),(i-1,j),(i-1,j+1),(i,j-1),(i,j+1),(i+1,j-1),(i+1,j),(i+1,j+1)];
            let mut counter = 0;
            for (k,l) in pos_to_check {
                if k >= 0 && k < (map.len() as i32) && l >= 0 && l < (map[0].len() as i32) {
                    if map[k as usize][l as usize] == '@' {
                        counter += 1;
                    }
                }
            } 
            if counter < 4 {
                res_1 += 1;
            }
        }
    }
    println!("Part 1: {res_1}");
}
