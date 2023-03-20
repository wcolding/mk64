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
    def symbol_found(name: str):
        return f' {name}\n' in line

    if symbol_found('gItemsUnlocked'):
        format_line('gItemsUnlocked', 'w', 'h', line)
    
    if symbol_found('gItemsArray'):
        format_line('gItemsArray', 'd', 'h', line)

    if symbol_found('gCharactersUnlocked'):
        format_line('gCharactersUnlocked', 'b', 'b', line)
    
    if symbol_found('gMenuSelection'):
        format_line('gMenuSelection', 'd', 'h', line)
        

output = io.open('build/us/mk64 modded.wch', 'w')
for line in watchlist_data:
    output.write(line + '\n')
output.close()
print("Watchlist updated")