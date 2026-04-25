# timein
timein is a minimal CLI tool for quickly getting the time and date in another city or timezone. This is my first C project after making fuckin everything in JavaScript

## Features
- Query by city: search for cities by name with fuzzy matching
- Query by UTC offset: directly specify timezone offsets (`+10`, `-4`, `5`)
- Lots of cities: [includes over 580 cities worldwide](listofcities.c)
- Dual time format: displays time in both 12 hour (a/pm) and 24 hour formats

## How it works
timein uses a database of cities mapped to their IANA timezone identifiers. I generated it using `timedatectl list-timezones | awk -F/ '{print $NF, $0}' | sort`

For city queries, it performs fuzzy string matching using a Levenshtein inspired distance algorithm to find the closest match

For UTC offsets, it directly calculates the time difference from UTC

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