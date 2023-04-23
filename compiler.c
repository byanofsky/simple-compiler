#include <stdio.h>
#include <stdlib.h>

void generate_ir(const char *input_file, const char *output_file)
{
  char command[256];
  snprintf(command, sizeof(command), "./generate_llvm_ir < %s > %s", input_file, output_file);
  printf("command: %s\n", command);
  int ret = system(command);
  if (ret != 0)
  {
    fprintf(stderr, "Error: Failed to generate LLVM IR\n");
    exit(1);
  }
}

void generate_executable(const char *input_file, const char *output_file)
{
  char command[256];
  snprintf(command, sizeof(command), "clang %s print_integer.c -o %s", input_file, output_file);
  printf("command: %s\n", command);
  int ret = system(command);
  if (ret != 0)
  {
    fprintf(stderr, "Error: Failed to compile LLVM IR to executable binary\n");
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
    exit(1);
  }
  char *temp_file = "temp.ll";
  generate_ir(argv[1], temp_file);
  generate_executable(temp_file, argv[2]);
  // printf("Executable binary generated: output\n");

  return 0;
}