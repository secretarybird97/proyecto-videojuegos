from glob import glob


def main():
    # Get all source files
    print("Script para eliminar 'SDL2/' de todos los include. Usar con cuidado.")
    files = glob("src/**/*.cpp", recursive=True) + glob("src/**/*.hpp", recursive=True)

    # Remove "SDL2/" from all #include lines
    for file in files:
        print(file)

    with open(file, "r") as f:
        lines = f.readlines()
        with open(file, "w") as f:
            for line in lines:
                if "#include" in line:
                    line = line.replace("SDL", "SDL2/SDL")
                f.write(line)


if __name__ == "__main__":
    main()
