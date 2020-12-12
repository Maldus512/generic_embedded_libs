import os

LIBRARY = './gel'
#os.listdir(LIBRARY)    #TODO: I would like to automatically list components here, but it won't work if the sconstruct is using a variantdir
components = [
    'collections', 'crc', 'debounce', 'keypad', 'pagemanager', 'pid',
    'protocols', 'serializer', 'timer', 'watcher', 'parameter'
]
sources = {}

Import('gel_selected', 'gel_env')

for c in components:
    sources[c] = Glob(os.path.join(LIBRARY, c, "*.c"))

objects = []
for s in gel_selected:
    objects += gel_env.Object(sources[s])

result = (objects, [os.getcwd()])
Return('result')