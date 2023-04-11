import QtQuick 2.12


QtObject {
    property QtObject objectList: QtObject {
        property var objects: []
        property var identifiers: []

        function append(id, obj) {
            obj.onVisibleChanged.connect( function(visible) {
                if(!visible) eraseObj(obj)
            })
            identifiers.push(id)
            objects.push(obj)
            identifiersChanged()
        }

        function erase(ind) {
            if(ind === -1) return
            objects[ind].destroy()

            objects.splice(ind, 1)
            identifiers.splice(ind, 1)

            identifiersChanged()
        }

        function eraseId(id) {
            erase(identifiers.indexOf(id))
        }

        function eraseObj(obj) {
            erase(objects.indexOf(obj))
        }
    }

    function release() {
        for(let i in objectList.objects)
            objectList.objects[i].close()
    }

    function releaseAt(id) {
        var ind = objectList.identifiers.indexOf(id)
        if(ind === -1)
            return
        objectList.objects[ind].close()
    }


    Component.onDestruction: {
        release()
    }

    readonly property int count: objectList.identifiers.length

    function find(id) {
        var index = objectList.identifiers.indexOf(id)
        return index === -1 ? null
                            : objectList.objects[index]
    }

    function contains(id) {
        return objectList.identifiers.indexOf(id) !== -1
    }

    function createUrl(id, url, root = null, params = null) {
        let ind = objectList.identifiers.indexOf(id)
        if(ind !== -1) return objectList.objects[ind]

        var component = Qt.createComponent(url)
        if(component === null) return null

        if(params === null)
            var obj = component.createObject(root)
        else obj = component.createObject(root, params)
        objectList.append(id, obj)
        return obj
    }

    function createComponent(id, component, root = null, params = null) {
        if(component === null) return null

        let ind = objectList.identifiers.indexOf(id)
        if(ind !== -1) return objectList.objects[ind]

        if(params === null)
            var obj = component.createObject(root)
        else obj = component.createObject(root, params)
        objectList.append(id, obj)
        return obj
    }

    function erase(id) {
        objectList.eraseId(id)
    }

    function eraseObject(obj) {
        objectList.eraseObj(obj)
    }
}


