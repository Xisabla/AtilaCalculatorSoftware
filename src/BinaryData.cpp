/*=========================================================================

  Project:   AtilaCalculatorSoftware
  File:      BinaryData.cpp

  Copyright (c) 2020
  All rights reserved.

=========================================================================*/
#include "BinaryData.h"

static int ByteOrderCheck = 0x91d;

int getFields(gzFile file, char* buffer, dataFields fields) {
    unsigned int size = 0;
    GZ_READ_CHECK(file, &size, 1);
    GZ_READ_CHECK(file, buffer, size);
    char* ptr = strtok(buffer, " ");
    while ((ptr[0] == '#' || !strcmp(ptr, "Unit") || size == 1) && size != 0) {
        GZ_READ_CHECK(file, &size, 1);
        GZ_READ_CHECK(file, buffer, size);
        ptr = strtok(buffer, " ");
    }
    if (!size) {
        throw result_exception { "No more data" };

        return 0;
    }
    size = 0;
    while (ptr != NULL) {
        if (ptr[0] == '"') {
            strcpy(fields[size], ptr);
            while (ptr[strlen(ptr) - 1] != '"') {
                ptr = strtok(NULL, " ");
                strcat(fields[size], ptr);
            }
            auto len = strlen(fields[size]);
            for (size_t i = 0; i < len - 2; ++i) {
                fields[size][i] = fields[size][i + 1];
            }
            fields[size][len - 2] = '\0';
            ++size;
        } else {
            strcpy(fields[size++], ptr);
        }
        ptr = strtok(NULL, " ");
    }
    return size;
}

Str_binary_data::~Str_binary_data() {
    gzclose(file_msh_);
    meshes_.clear();
    results_.clear();
    std::cout << "clearing ... " << std::endl;
}

Str_binary_data::Str_binary_data(string file): file_msh_(gzopen((file).c_str(), "r1")) {

    char buffer[GZ_BUFFER_SIZE];
    int size;

    //	gzFile file_msh = gzopen((file).c_str(), "r1");
    if (!file_msh_) {
        printf("Can't read msh file %s\n", buffer);
        exit(1);
    }

    int ret = gzread(file_msh_, &size, sizeof(ByteOrderCheck));

    if (ret && ByteOrderCheck != size) {
        throw string("Cannot read binary file (Mesh)");
    }

    ret = gzread(file_msh_, &size, sizeof(int));
    if (ret == 0) {
        throw string("Cannot read binary file (Mesh)");
    }

    size *= sizeof(char);

    ret = gzread(file_msh_, buffer, size);
    if (ret == 0) {
        throw string("Cannot read binary file (Mesh)");
    }

    /*
    My_fields fields;

    size = get_fields(file_msh, buffer, fields);
    while (size > 0) {
        if (!strcmp(fields[0], "MESH")) {
            meshes.emplace_back(file_msh, fields);
        }

        if (!strcmp(fields[0], "Result")) {
            if (!strcmp(fields[4], "Vector")) {
                results.emplace_back(Str_result(file_msh, fields, 4));
            }
            else {
                if (!strcmp(fields[4], "Scalar")) {
                    results.emplace_back(Str_result(file_msh, fields, 1));
                }
                else {
                    if (!strcmp(fields[4], "Matrix")) {
                        results.emplace_back(Str_result(file_msh, fields, 6));
                    }
                }
            }
        }
        size = getFields(file_msh, buffer, fields);
    }
     */
}

bool Str_binary_data::read_meshes() {
    char buffer[GZ_BUFFER_SIZE];
    int size;

    dataFields fields;
    z_off_t current_position = gztell(file_msh_);
    size = getFields(file_msh_, buffer, fields);
    while (size > 0 && !strcmp(fields[0], "MESH")) {
        meshes_.emplace_back(file_msh_, fields);
        meshes_read_ = true;
        current_position = gztell(file_msh_);
        size = getFields(file_msh_, buffer, fields);
    }
    if (gzseek(file_msh_, current_position, SEEK_SET) == -1) {
        throw string("Cannot read binary file (Mesh)");
    }

    result_position_ = current_position;
    return meshes_read_;
}

std::optional<Str_Result> Str_binary_data::read_one_result() {
    if (Str_Mesh::number_of_nodes_max_ > 0) {
        char buffer[GZ_BUFFER_SIZE];
        dataFields fields;
        if (getFields(file_msh_, buffer, fields)) {
            if (!strcmp(fields[0], "Result")) {
                if (!strcmp(fields[4], "Vector")) {
                    return Str_Result(file_msh_, fields, 4);
                } else {
                    if (!strcmp(fields[4], "Scalar")) {
                        return Str_Result(file_msh_, fields, 1);
                    } else {
                        if (!strcmp(fields[4], "Matrix")) {
                            return Str_Result(file_msh_, fields, 6);
                        }
                    }
                }
            }
        }
    }
    return std::nullopt;
}
