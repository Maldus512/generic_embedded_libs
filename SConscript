import os

LIBRARY = 'gel.a'
# os.listdir('gel')    #TODO: I would like to automatically list components here, but it won't work if the sconstruct is using a variantdir
components = [
    'collections', 'crc', 'debounce', 'keypad', 'pagemanager', 'pid', "wearleveling",
    'protocols', 'serializer', 'timer', 'watcher', 'parameter', 'data_structures', "circular_event_log"
]
selected = {}

Import('gel_selected', 'gel_env')

for c in components:
    selected[c] = Glob(os.path.join('gel', c, "*.c"))

sources = []
for s in gel_selected:
    sources += gel_env.Object(selected[s])

lib = gel_env.Library(LIBRARY, sources)

result = (lib, [os.getcwd()])
Return('result')
