import os

LIBRARY = './gel'
#os.listdir(LIBRARY)    #TODO: I would like to automatically list components here, but it won't work if the sconstruct is using a variantdir
components = [
    'collections', 'crc', 'debounce', 'keypad', 'pagemanager', 'pid',
    'protocols', 'serializer', 'timer', 'watcher'
]
sources = {}

Import('selected', 'env')

for c in components:
    sources[c] = Glob(os.path.join(LIBRARY, c, "*.c"))

objects = []
for s in selected:
    objects += env.Object(sources[s])
Return('objects')