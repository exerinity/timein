# timein
timein is a minimal CLI tool for quickly getting the time and date in another city or timezone. This is my first C project after making fuckin everything in JavaScript

## Installation

### AUR
```bash
yay -S timein
```

### Manual
```bash
git clone https://github.com/exerinity/timein
cd timein
make
sudo install -Dm755 timein /usr/local/bin/timein # if you want to run it from anywhere
```

## Usage

```bash
timein <area>
```

```bash
timein tokyo
timein tok # full name not required, might still resolve
timein melbourne
timein new york

timein +10 # UTC+10
timein -4 # UTC-4
timein 5 # UTC+5; a plus is not required for forward-facing zones

timein about # info
```

## License
[MIT](LICENSE)