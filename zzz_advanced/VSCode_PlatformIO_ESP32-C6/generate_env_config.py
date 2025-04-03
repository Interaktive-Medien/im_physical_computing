import os

def generate_header(env_file, header_file):
    with open(env_file, 'r') as f:
        lines = f.readlines()

    with open(header_file, 'w') as f:
        f.write('#ifndef ENV_CONFIG_H\n')
        f.write('#define ENV_CONFIG_H\n\n')
        for line in lines:
            if '=' in line:
                key, value = line.strip().split('=', 1)
                f.write(f'#define {key} "{value}"\n')
        f.write('\n#endif // ENV_CONFIG_H\n')

if __name__ == "__main__":
    env_file = '../.env'
    header_file = 'src/env_config.h'
    generate_header(env_file, header_file)