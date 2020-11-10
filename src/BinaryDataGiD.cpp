/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryDataGiD.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "BinaryDataGiD.h"

Str_binary_data_GiD::Str_binary_data_GiD(std::string file): Str_binary_data(file) { }


void Str_binary_data_GiD::write_one_step_to_post_gid_file(const float& step,
                                                          const int& number_frame) {
    while (auto one_result = read_one_result()) {
        if (fabs(one_result->step_ - step) / fabs(one_result->step_ + step) < 1.e-4) {
            results_.emplace_back(std::move(*one_result));
        }
    }

    for (auto result = results_.begin(); result != results_.end(); ++result) {
        if (std::string::npos != result->results_.find("Harmonic")) {
            if (std::string::npos != result->results_.find("Real")) {
                auto& my_real = result;
                auto& my_imag = ++result;
                if ((std::string::npos == my_imag->results_.find("Imag")) ||
                    (my_real->result_size_ != my_imag->result_size_) ||
                    (my_real->number_of_results_ != my_imag->number_of_results_)) {
                    throw std::string("Cannot read binary file (Result) !");
                }
                switch (my_real->result_size_) {
                    case 1: {
                        //	const char *component_names = my_real->component_names_[0].c_str();
                        auto number_frame = 20;
                        for (auto i = 0; i < number_frame; ++i) {
                            auto theta = -2.0 * M_PI * (double) i / (double) (number_frame);
                            GiD_BeginResult(my_real->analysis_.c_str(),
                                            "Animate",
                                            i + 1,
                                            GiD_Scalar,
                                            GiD_OnNodes,
                                            NULL,
                                            NULL,
                                            0,
                                            NULL /*&component_names*/);
                            for (auto j = 0; j < my_real->number_of_results_; ++j) {
                                auto [node_number_real, data_real] = my_real->get_one_result(j);
                                auto [node_number_imag, data_imag] = my_imag->get_one_result(j);
                                if (node_number_real != node_number_imag) {
                                    throw std::string("Cannot read binary file (Result)");
                                }
                                if (*data_real != GP_UNKNOWN) {
                                    auto square_dispx =
                                    (float) sqrt(*data_real * *data_real + *data_imag * *data_imag);
                                    GiD_WriteScalar(node_number_real,
                                                    square_dispx *
                                                    sin(atan2(*data_imag, *data_real) + theta));
                                } else {
                                    GiD_WriteScalar(node_number_real, GP_UNKNOWN);
                                }
                            }
                            GiD_EndResult();
                        }
                    } break;
                    case 4: {
                        /*
                        const char *component_names[3] {
                            my_real->component_names_[0].c_str(),
                            my_real->component_names_[1].c_str(),
                            my_real->component_names_[2].c_str()
                        };
                         */
                        auto number_frame = 20;
                        for (auto i = 0; i < number_frame; ++i) {
                            auto theta = -2.0 * M_PI * (double) i / (double) (number_frame);
                            GiD_BeginResult(my_real->analysis_.c_str(),
                                            "Animate",
                                            i + 1,
                                            GiD_Vector,
                                            GiD_OnNodes,
                                            NULL,
                                            NULL,
                                            0,
                                            NULL /*component_names*/);
                            for (auto j = 0; j < my_real->number_of_results_; ++j) {
                                auto [node_number_real, data_real] = my_real->get_one_result(j);
                                auto [node_number_imag, data_imag] = my_imag->get_one_result(j);
                                if (node_number_real != node_number_imag) {
                                    throw std::string("Cannot read binary file (Result) !");
                                }

                                if (*data_real != GP_UNKNOWN) {
                                    auto results1_r = data_real[0];
                                    auto results2_r = data_real[1];
                                    auto results3_r = data_real[2];
                                    auto results1_i = data_imag[0];
                                    auto results2_i = data_imag[1];
                                    auto results3_i = data_imag[2];
                                    auto square_dispx =
                                    (float) sqrt(results1_r * results1_r + results1_i * results1_i);
                                    auto square_dispy =
                                    (float) sqrt(results2_r * results2_r + results2_i * results2_i);
                                    auto square_dispz =
                                    (float) sqrt(results3_r * results3_r + results3_i * results3_i);
                                    GiD_WriteVector(
                                    node_number_real,
                                    square_dispx * sin(atan2(results1_i, results1_r) + theta),
                                    square_dispy * sin(atan2(results2_i, results2_r) + theta),
                                    square_dispz * sin(atan2(results3_i, results3_r) + theta));
                                } else
                                    GiD_WriteVectorModule(node_number_real,
                                                          GP_UNKNOWN,
                                                          GP_UNKNOWN,
                                                          GP_UNKNOWN,
                                                          GP_UNKNOWN);
                            }
                            GiD_EndResult();
                        }
                    } break;
                }
            } else {
                if (std::string::npos == result->results_.find("Magn")) {
                    switch (result->result_size_) {
                        case 1: {
                            //				const char *component_names =
                            // result->component_names_[0].c_str();
                            auto number_frame = 20;
                            for (auto i = 0; i < number_frame; ++i) {
                                auto theta = -2.0 * M_PI * (double) i / (double) (number_frame);
                                auto stheta = sin(theta);
                                GiD_BeginResult(result->analysis_.c_str(),
                                                "Animate",
                                                i + 1,
                                                GiD_Scalar,
                                                GiD_OnNodes,
                                                NULL,
                                                NULL,
                                                0,
                                                NULL /* 1, &component_names*/);
                                for (auto j = 0; j < result->number_of_results_; ++j) {
                                    auto [node_number_real, data_real] = result->get_one_result(j);
                                    if (*data_real != GP_UNKNOWN) {
                                        GiD_WriteScalar(node_number_real, *data_real * stheta);
                                    } else {
                                        GiD_WriteScalar(node_number_real, GP_UNKNOWN);
                                    }
                                }
                                GiD_EndResult();
                            }
                        } break;
                        case 4: {
                            /*
                        const char *component_names[3] {result->component_names_[0].c_str(),
                            result->component_names_[1].c_str(),
                            result->component_names_[2].c_str()};
                             */
                            auto number_frame = 20;
                            for (auto i = 0; i < number_frame; ++i) {
                                auto theta = -2.0 * M_PI * (double) i / (double) (number_frame);
                                auto stheta = sin(theta);
                                GiD_BeginResult(result->analysis_.c_str(),
                                                "Animate",
                                                i + 1,
                                                GiD_Vector,
                                                GiD_OnNodes,
                                                NULL,
                                                NULL,
                                                0,
                                                NULL /*component_names*/);
                                for (auto j = 0; j < result->number_of_results_; ++j) {
                                    auto [node_number_real, data_real] = result->get_one_result(j);
                                    if (*data_real != GP_UNKNOWN) {
                                        auto results1_r = data_real[0];
                                        auto results2_r = data_real[1];
                                        auto results3_r = data_real[2];
                                        GiD_WriteVector(node_number_real,
                                                        results1_r * stheta,
                                                        results2_r * stheta,
                                                        results3_r * stheta);

                                    } else
                                        GiD_WriteVectorModule(node_number_real,
                                                              GP_UNKNOWN,
                                                              GP_UNKNOWN,
                                                              GP_UNKNOWN,
                                                              GP_UNKNOWN);
                                }
                                GiD_EndResult();
                            }
                        } break;
                    }
                }
            }
        }
    }
    results_.clear();
}


void Str_binary_data_GiD::write_meshes() {
    for (auto& mesh: meshes_) {
        write_to_post_gid_file(mesh);
    }
}

// bool Str_binary_data::read_all_results(){
//	bool one_result_read = false;
//	while(gzeof(file_msh_)){
//		this->results.emplace_back(read_one_result());
//		one_result_read = true;
//	}
//	return one_result_read;
//}
