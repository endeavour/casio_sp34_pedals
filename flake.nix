{
  description = "ESP32 ADC Monitor";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        platformioEnv = pkgs.mkShell {
          name = "platformio-env";
          VIRTUAL_ENV = "/tmp/platformio-env";
          buildInputs = with pkgs; [
            python3
            python3Packages.virtualenv
          ];
          shellHook = ''
            if [ ! -d "$VIRTUAL_ENV" ]; then
              python -m venv "$VIRTUAL_ENV"
              source "$VIRTUAL_ENV/bin/activate"
              pip install platformio
            else
              source "$VIRTUAL_ENV/bin/activate"
            fi
            export PATH="$VIRTUAL_ENV/bin:$PATH"
          '';
        };
      in
      {
        devShells.default = platformioEnv;
      }
    );
}
