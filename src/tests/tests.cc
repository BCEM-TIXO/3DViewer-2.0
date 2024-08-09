#include <gtest/gtest.h>
#include <vector>

#include "../model/Object.h"

class CubeObj : public testing::Test {
    protected:
    s21::Object cube;
    std::vector<float> cube_values = {
        0.0, 0.0, 0.0, 1.0,
        0.0, 0.0, 0.5, 1.0,
        0.0, 0.5, 0.0, 1.0,
        0.0, 0.5, 0.5, 1.0,
        0.5, 0.0, 0.0, 1.0,
        0.5, 0.0, 0.5, 1.0,
        0.5, 0.5, 0.0, 1.0,
        0.5, 0.5, 0.5, 1.0
    };

    void SetUp() override {
        cube.LoadFromFile("./objs/cube.obj");
    }
};

TEST_F(CubeObj, Load) {
    s21::Matrix A(8, 4);
    
    A.FillMatrix(cube_values);

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == A);
}

TEST_F(CubeObj, MoveX) {
    cube.MoveOneAxis(s21::X, 8);
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);
    
    std::vector<float> b_values = {
        1.0, 0.0, 0.0, 8.0, 
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, MoveY) {
    cube.MoveOneAxis(s21::Y, 8);
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        1.0, 0.0, 0.0, 0.0, 
        0.0, 1.0, 0.0, 8.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, MoveZ) {
    cube.MoveOneAxis(s21::Z, 4.56);
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        1.0, 0.0, 0.0, 0.0, 
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 4.56,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, MoveAll) {
    cube.MoveOneAxis(s21::X, 0.2);
    cube.MoveOneAxis(s21::Y, 3.009);
    cube.MoveOneAxis(s21::Z, 10.1);
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        1.0, 0.0, 0.0, 0.2, 
        0.0, 1.0, 0.0, 3.009,
        0.0, 0.0, 1.0, 10.1,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, Scale1) {
    cube.ScaleObject(2);
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        2.0, 0.0, 0.0, 0.0, 
        0.0, 2.0, 0.0, 0.0,
        0.0, 0.0, 2.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, Scale2) {
    cube.ScaleObject(0.4);
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        0.4, 0.0, 0.0, 0.0, 
        0.0, 0.4, 0.0, 0.0,
        0.0, 0.0, 0.4, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, RotateX) {
    cube.RotateOneAxis(s21::X, 30);
    float angle = 30 * 0.01745329251;
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        1.0, 0.0, 0.0, 0.0, 
        0.0, cosf(angle), -sinf(angle), 0.0,
        0.0, sinf(angle), cosf(angle), 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, RotateY) {
    cube.RotateOneAxis(s21::Y, 30);
    float angle = 30 * 0.01745329251;
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        cosf(angle), 0.0, sinf(angle), 0.0, 
        0.0, 1.0, 0.0, 0.0,
        -sinf(angle), 0.0, cosf(angle), 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

TEST_F(CubeObj, RotateZ) {
    cube.RotateOneAxis(s21::Z, 30);
    float angle = 30 * 0.01745329251;
    s21::Matrix A(8, 4);
    s21::Matrix B(4, 4);

    std::vector<float> b_values = {
        cosf(angle), -sinf(angle), 0.0, 0.0, 
        sinf(angle), cosf(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };
    A.FillMatrix(cube_values);
    B.FillMatrix(b_values);

    s21::Matrix A_transposed = A.Transpose();
    s21::Matrix C = B * A_transposed;
    s21::Matrix C_transposed = C.Transpose();

    s21::Matrix cube_matrix = *(cube.GetMatrix());
    EXPECT_TRUE(cube_matrix == C_transposed);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
