//
//     Copyright (C) Pixar. All rights reserved.
//
//     This license governs use of the accompanying software. If you
//     use the software, you accept this license. If you do not accept
//     the license, do not use the software.
//
//     1. Definitions
//     The terms "reproduce," "reproduction," "derivative works," and
//     "distribution" have the same meaning here as under U.S.
//     copyright law.  A "contribution" is the original software, or
//     any additions or changes to the software.
//     A "contributor" is any person or entity that distributes its
//     contribution under this license.
//     "Licensed patents" are a contributor's patent claims that read
//     directly on its contribution.
//
//     2. Grant of Rights
//     (A) Copyright Grant- Subject to the terms of this license,
//     including the license conditions and limitations in section 3,
//     each contributor grants you a non-exclusive, worldwide,
//     royalty-free copyright license to reproduce its contribution,
//     prepare derivative works of its contribution, and distribute
//     its contribution or any derivative works that you create.
//     (B) Patent Grant- Subject to the terms of this license,
//     including the license conditions and limitations in section 3,
//     each contributor grants you a non-exclusive, worldwide,
//     royalty-free license under its licensed patents to make, have
//     made, use, sell, offer for sale, import, and/or otherwise
//     dispose of its contribution in the software or derivative works
//     of the contribution in the software.
//
//     3. Conditions and Limitations
//     (A) No Trademark License- This license does not grant you
//     rights to use any contributor's name, logo, or trademarks.
//     (B) If you bring a patent claim against any contributor over
//     patents that you claim are infringed by the software, your
//     patent license from such contributor to the software ends
//     automatically.
//     (C) If you distribute any portion of the software, you must
//     retain all copyright, patent, trademark, and attribution
//     notices that are present in the software.
//     (D) If you distribute any portion of the software in source
//     code form, you may do so only under this license by including a
//     complete copy of this license with your distribution. If you
//     distribute any portion of the software in compiled or object
//     code form, you may only do so under a license that complies
//     with this license.
//     (E) The software is licensed "as-is." You bear the risk of
//     using it. The contributors give no express warranties,
//     guarantees or conditions. You may have additional consumer
//     rights under your local laws which this license cannot change.
//     To the extent permitted under your local laws, the contributors
//     exclude the implied warranties of merchantability, fitness for
//     a particular purpose and non-infringement.
//
#ifndef OSD_MESH_H
#define OSD_MESH_H

#include <string>
#include <vector>

#include "../version.h"
#include "../hbr/mesh.h"
#include "../hbr/vertex.h"
#include "../hbr/face.h"
#include "../hbr/halfedge.h"

#include "../far/mesh.h"
#include "../far/meshFactory.h"

#include "../osd/vertex.h"
#include "../osd/vertexBuffer.h"
#include "../osd/kernelDispatcher.h"

namespace OpenSubdiv {
namespace OPENSUBDIV_VERSION {

class OsdKernelDispatcher;

typedef HbrMesh<OsdVertex>     OsdHbrMesh;
typedef HbrVertex<OsdVertex>   OsdHbrVertex;
typedef HbrHalfedge<OsdVertex> OsdHbrHalfedge;
typedef HbrFace<OsdVertex>     OsdHbrFace;

class OsdPtexIndicesBuffer;

class OsdMesh {

public:
    OsdMesh();

    virtual ~OsdMesh();

    // Given a valid HbrMesh, create an OsdMesh
    //   - capable of densely refining up to 'level'
    //   - subdivision kernel one of (kCPU, kOPENMP, kCUDA, kGLSL, kCL)
    //   - optional "remapping" vector that connects Osd and Hbr vertex indices
    //     (for regression)
    bool Create(OsdHbrMesh *hbrMesh, int level, int kernel, std::vector<int> * remap=0);

    FarMesh<OsdVertex> *GetFarMesh() { return _farMesh; }

    int GetLevel() const { return _level; }

    OsdVertexBuffer * InitializeVertexBuffer(int numElements);

    // for non-interleaved vertex data
    void Subdivide(OsdVertexBuffer *vertex, OsdVertexBuffer *varying = NULL);

/*
    // for interleaved vertex data ?
    template <class T> void Subdivide(T *vertex) { }
*/

    void Synchronize();

    int GetTotalVertices() const { return _farMesh->GetNumVertices(); }

    int GetNumCoarseVertices() const { return _farMesh->GetNumCoarseVertices(); }

    // Returns the texture buffer containing the ptex face index for each face of
    // the mesh.
    GLuint GetPtexCoordinatesTextureBuffer(int level) const { return _ptexCoordinates[level-1]; }
    
protected:

    void createTables( FarSubdivisionTables<OsdVertex> const * tables );

    void createEditTables( FarVertexEditTables<OsdVertex> const * editTables );

    FarMesh<OsdVertex> *_farMesh;

    int _level;

    OsdKernelDispatcher * _dispatcher;

    std::vector<GLuint> _ptexCoordinates;  // index of the coarse parent face + sub-face coordinates (cf. far)

};

} // end namespace OPENSUBDIV_VERSION
using namespace OPENSUBDIV_VERSION;

} // end namespace OpenSubdiv

#endif /* OSD_MESH_H */
