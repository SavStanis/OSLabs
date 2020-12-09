package com.savstanis.lab4;

public class Main {

    public static void main(String[] args) {
        int[][] matrix = {
                {32, 28, 4, 26, 4},
                {17, 19, 4, 17, 4},
                {4, 4, 5, 4, 4},
                {17, 14, 4, 14, 4},
                {21, 16, 4, 13, 4}
        };

        int[][] result = HungarianAlgorithm.run(matrix);

        HungarianAlgorithm.print(result);
        System.out.println("Result: " + calculateResult(result));
    }

    public static int calculateResult(int[][] matrix) {
        int sum = 0;

        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[0].length; j++) {
                sum += matrix[i][j];
            }
        }
        return sum;
    }
}
