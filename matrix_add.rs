use std::io;

fn main() {
    // 1. 행렬 크기 입력
    let (rows, cols) = input_matrix_dimensions();

    // 2. 첫 번째 행렬 입력
    println!("Enter the elements of the first matrix:");
    let matrix_a = input_matrix(rows, cols);

    // 3. 두 번째 행렬 입력
    println!("Enter the elements of the second matrix:");
    let matrix_b = input_matrix(rows, cols);

    // 4. 행렬 덧셈 수행
    let result_matrix = add_matrices(&matrix_a, &matrix_b, rows, cols);

    // 5. 결과 출력
    println!("Resultant matrix after addition:");
    print_matrix(&result_matrix, rows, cols);
}

fn input_matrix_dimensions() -> (usize, usize) {
    let mut input = String::new();
    println!("Enter the number of rows and columns (e.g., 3 4):");
    io::stdin().read_line(&mut input).unwrap();
    let dims: Vec<usize> = input
        .trim()
        .split_whitespace()
        .map(|x| x.parse().unwrap())
        .collect();
    (dims[0], dims[1])
}

fn input_matrix(rows: usize, cols: usize) -> Vec<Vec<i32>> {
    let mut matrix = Vec::new();
    for i in 0..rows {
        let mut row = Vec::new();
        println!("Enter the elements of row {} separated by spaces:", i + 1);
        let mut input = String::new();
        io::stdin().read_line(&mut input).unwrap();
        let values: Vec<i32> = input
            .trim()
            .split_whitespace()
            .map(|x| x.parse().unwrap())
            .collect();
        row.extend(values);
        matrix.push(row);
    }
    matrix
}

fn add_matrices(
    matrix_a: &Vec<Vec<i32>>,
    matrix_b: &Vec<Vec<i32>>,
    rows: usize,
    cols: usize,
) -> Vec<Vec<i32>> {
    let mut result = Vec::new();
    for i in 0..rows {
        let mut row = Vec::new();
        for j in 0..cols {
            row.push(matrix_a[i][j] + matrix_b[i][j]);
        }
        result.push(row);
    }
    result
}

fn print_matrix(matrix: &Vec<Vec<i32>>, rows: usize, cols: usize) {
    for i in 0..rows {
        for j in 0..cols {
            print!("{} ", matrix[i][j]);
        }
        println!();
    }
}

