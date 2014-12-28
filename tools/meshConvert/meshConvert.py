import os, sys
from xml.dom import minidom
from pyassimp import pyassimp

def attribute(doc, name, data):
    attribute = doc.createElement('attribute')
    attribute.setAttribute('id', name)
    for index, item in enumerate(data):
        components = len(item._fields_)
        typename = type(item[0]).__name__
        position = doc.createElement(typename + str(components))
        for fieldIndex, field in enumerate(item._fields_):
            position.setAttribute(field[0], str(item[fieldIndex]))
        attribute.appendChild(position)
    return attribute

# aiProcess_GenUVCoords | aiProcess_OptimizeMeshes | aiProcess_SortByPType | aiProcess_Triangulate
processing = 0x40000 | 0x200000 | 0x8000 | 0x8
scene = pyassimp.load(sys.argv[1], 8)

doc = minidom.Document()
obj = doc.createElement('object')

intAttrName = ['i','j','k','l']

for index, mesh in enumerate(scene.meshes):
    meshElement = doc.createElement('mesh')
    meshElement.setAttribute('index', str(index))
    # vertices
    positions = attribute(doc, 'position', mesh.vertices)
    meshElement.appendChild(positions)
    # normals
    if any(mesh.normals):
        normals = attribute(doc, 'normal', mesh.normals)
        meshElement.appendChild(normals)
    # texture coordinates
    for texIndex, texCoords in enumerate(mesh.texcoords):
        if any(texCoords):
            name = 'texture[%d]' % texIndex
            tElement = attribute(doc, name, texCoords)
            meshElement.appendChild(tElement)
    # faces
    if any(mesh.faces):
        faces = doc.createElement('element');
        for faceIndex, face in enumerate(mesh.faces):
            typename = 'int' + str(face.mNumIndices)
            faceElement = doc.createElement(typename)
            for i in range(0, face.mNumIndices):
                faceElement.setAttribute(intAttrName[i], str(face.indices[i]))
            faces.appendChild(faceElement)
        meshElement.appendChild(faces)
    obj.appendChild(meshElement)

# [todo] materials
# [todo] bones

doc.appendChild(obj)

print doc.toprettyxml()

pyassimp.release(scene)
