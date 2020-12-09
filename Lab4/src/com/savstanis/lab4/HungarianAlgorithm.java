package com.savstanis.lab4;

import java.util.*;

public class HungarianAlgorithm {

    public static int[][]run(int[][] initMatrix) {
        int[][] copy = new int[initMatrix.length][initMatrix[0].length];
        boolean[][] marksArray = new boolean[initMatrix.length][initMatrix[0].length];

        for (int i = 0; i < copy.length; i++) {
            copy[i] = Arrays.copyOf(initMatrix[i], initMatrix[i].length);
        }

        boolean[] rowMarks = new boolean[initMatrix.length];
        boolean[] columnMarks = new boolean[initMatrix[0].length];

        prepareMatrix(copy);
        getMaxMatching(copy, marksArray);

        markZeroColumns(marksArray, columnMarks);

        //print(copy);
        //print(marksArray);

        while (!allTasksDistributed(columnMarks)) {
            markZeroRows(copy, marksArray, rowMarks , columnMarks);

            subtractMinFromUncoveredAndAddToCrossed(copy, rowMarks, columnMarks);

            resetMarksArray(marksArray);
            getMaxMatching(copy, marksArray);

            markZeroColumns(marksArray, columnMarks);

            //System.out.println("------------------------------------\nNew step\n------------------------------------");
            //print(copy);
            //print(marksArray);
        }

        return getResultArray(initMatrix, marksArray);
    }


    private static void prepareMatrix(int[][] matrix) {

        for (int i = 0; i < matrix.length; i++) {
            int rowMin = getMinInRow(matrix, i);

            for (int j = 0; j < matrix[i].length; j++) {
                matrix[i][j] -= rowMin;
            }
        }

        for (int i = 0; i < matrix[0].length; i++) {
            int columnMin = getMinInColumn(matrix, i);

            for (int j = 0; j < matrix.length; j++) {
                matrix[j][i] -= columnMin;
            }
        }
    }

    private static void getMaxMatching(int[][] matrix, boolean[][] marksArray) {
        int rows = matrix.length;
        int columns = matrix[0].length;

        boolean[] rowMarks = new boolean[rows];
        boolean[] colMarks = new boolean[columns];


        for (int i = 0; i < columns; i++) {
            for (int j = 0; j < rows; j++) {
                if (matrix[j][i] == 0 && !rowMarks[j] && !colMarks[i]) {

                    rowMarks[j] = true;
                    colMarks[i] = true;

                    marksArray[j][i] = true;

                    break;
                }
            }
        }
    }

    private static void markZeroColumns(boolean[][] marksArray, boolean[] columnMarks) {
        int rowsNumber = marksArray.length;

        for (int col = 0; col < columnMarks.length; col++) {
            columnMarks[col] = false;

            for (boolean[] booleans : marksArray) {
                if (booleans[col]) {
                    columnMarks[col] = true;
                    break;
                }
            }
        }
    }

    private static void markZeroRows(int[][] matrix, boolean[][] marksArray, boolean[] rowMarks, boolean[] columnMarks) {
        int columnNumber = marksArray[0].length;

        for (int row = 0; row < rowMarks.length; row++) {
            rowMarks[row] = false;

            for (int column = 0; column < columnNumber; column++) {
                if (matrix[row][column] == 0 && !marksArray[row][column] && !columnMarks[column]) {
                    rowMarks[row] = true;
                    break;
                }
            }
        }
    }

    private static int getMinAmongUncovered(int[][] matrix, boolean[] rowMarks, boolean[] columnMarks) {
        int min = Integer.MAX_VALUE;

        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[0].length; j++) {
                if (!rowMarks[i] && !columnMarks[j] && matrix[i][j] < min) {
                    min = matrix[i][j];
                }
            }
        }

        return  min;
    }

    private static void subtractMinFromUncoveredAndAddToCrossed(int[][] matrix, boolean[] rowMarks, boolean[] columnMarks) {
        int min = getMinAmongUncovered(matrix, rowMarks, columnMarks);

        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[0].length; j++) {
                if (!rowMarks[i] && !columnMarks[j]) {
                    matrix[i][j] -= min;
                }
                if (rowMarks[i] && columnMarks[j]) {
                    matrix[i][j] += min;
                }
            }
        }
    }

    private static boolean allTasksDistributed(boolean[] columnMarks) {
        for (int j = 0; j < columnMarks.length; j++) {
            if (!columnMarks[j]) {
                return false;
            }
        }
        return true;
    }

    private static void resetMarksArray(boolean[][] marksArray) {
        for (int i = 0; i < marksArray.length; i++) {
            for (int j = 0; j < marksArray[0].length; j++) {
                marksArray[i][j] = false;
            }
        }
    }

    private static int[][] getResultArray(int[][] matrix, boolean[][] marksArray) {
        int[][] result = new int[matrix.length][matrix[0].length];

        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[0].length; j++) {
                if (marksArray[i][j]) {
                    result[i][j] = matrix[i][j];
                }
            }
        }

        return result;
    }

    public static void print(int[][] matrix) {
        for (int[] row: matrix) {
            for (int i: row) {
                System.out.print(i + "\t");
            }
            System.out.println();
        }
        System.out.println();
    }

    public static void print(boolean[][] matrix) {

        System.out.println("Marked zeroes:");
        for (boolean[] row: matrix) {
            for (boolean i: row) {
                if (i) {
                    System.out.print(1 + "\t");
                } else {
                    System.out.print(0 + "\t");
                }
            }
            System.out.println();

        }
        System.out.println();
    }

    private static int getMinInRow(int[][] matrix, int r) {
        int min = Integer.MAX_VALUE;

        for (int i: matrix[r]) {
            if (i < min) {
                min = i;
            }
        }

        return min;
    }

    private static int getMinInColumn(int[][] matrix, int c) {
        int min = Integer.MAX_VALUE;

        for (int[] ints : matrix) {
            if (ints[c] < min) {
                min = ints[c];
            }
        }

        return min;
    }

}
