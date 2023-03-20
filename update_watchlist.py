import io

map_file = io.open('build/us/mk64.us.map', 'r')

lines = map_file.readlines()
map_file.close()

watchlist_data = ['SystemID\tN64']

def get_offset(line: str):
    start = line.find("0x")
    trimmed = line[start:]
    end = trimmed.find(" ")
    trimmed = trimmed[0:end]
    trimmed = trimmed[-6:]
    return trimmed

def format_line(name: str, type: str, display: str, line: str):
    offset = get_offset(line)
    formatted = f'{offset}\t{type}\t{display}\t1\tRDRAM\t{name}'
    global watchlist_data
    watchlist_data.append(formatted)

for line in lines:
    if 'gItemsUnlocked' in line:
        format_line('gItemsUnlocked', 'w', 'h', line)
    
    if 'gItemsArray' in line:
        format_line('gItemsArray', 'd', 'h', line)

    if 'gCharactersUnlocked' in line:
        format_line('gCharactersUnlocked', 'b', 'b', line)

output = io.open('build/us/mk64 modded.wch', 'w')
for line in watchlist_data:
    output.write(line + '\n')
output.close()
print("Watchlist updated")