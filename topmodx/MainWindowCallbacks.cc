/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

/* $Id: MainWindowCallbacks.cc,v 4.13 2004/01/20 22:18:36 vinod Exp $ */

// Definitions for callback functions in the MainWindow class
// All these are static methods

#include "MainWindow.h"

void MainWindow::load_texture()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, tr("Open File..."), "$HOME",
		tr("All Supported Files (*.jpg *.png);;JPEG Files (*.jpg);;PNG Files (*.png);;All Files (*)"),
		0, QFileDialog::DontUseSheet);

	if (!fileName.isEmpty())
	{
		// undoPush();
		QByteArray ba = fileName.toLatin1();
		const char *texfile = ba.data();
		if (texfile != nullptr)
		{
			sTexturedRenderer->setTexture(texfile);
			sTexturedLitRenderer->setTexture(texfile);
			// getActive()->valid(0); // To ensure new texture is loaded into OpenGL
			redraw();
		}
		// readObject(filename);
		// active->recomputePatches();
		// active->recomputeNormals();
		// //loadFile(fileName);
		// active->redraw();
	}
}

// void DLFLAppWindow::load_texture(Fl_Menu_*, void*)
// {
//   char * texfile = fl_file_chooser("Load Texture","*.{jpg,png}","");
//   if (texfile != nullptr)
//      {
//        textured->setTexture(texfile);
//        texturedlit->setTexture(texfile);
//        mainWindowPtr->getActive()->valid(0); // To ensure new texture is loaded into OpenGL
//        mainWindowPtr->redraw();
//        Fl::focus(mainWindowPtr);
//      }
// }

//use a specific renderer...
void MainWindow::useWireframeRenderer()
{
	setRenderer(sWiredRenderer);
	redraw();
}

void MainWindow::useNormalRenderer()
{
	setRenderer(sNormalRenderer);
	redraw();
}

void MainWindow::useLightedRenderer()
{
	mActiveViewport->recomputeNormals();
	setRenderer(sLitRenderer);
	redraw();
}

void MainWindow::useTexturedRenderer()
{
	setRenderer(sTexturedRenderer);
	redraw();
}

void MainWindow::useTexturedLitRenderer()
{
	setRenderer(sTexturedLitRenderer);
	redraw();
}

void MainWindow::usePatchRenderer()
{
	setRenderer(sPatchRenderer);
	redraw();
}

void MainWindow::useColorableRenderer()
{
	setRenderer(sColorableRendererr);
	redraw();
}

//preference functions for topmodpreferences - dave
void MainWindow::setAutoSave(int value)
{
	//start the timer if it's ON
	//stop timer if it's OFF
	mAutoSave = bool(value);
	if (mAutoSave)
		mAutoSaveTimer->start();
	else
		mAutoSaveTimer->stop();
}

//this value is sent in minutes... so multiply it by 6000!
void MainWindow::setAutoSaveDelay(double value)
{
	mAutoSaveDelay = value;
	mAutoSaveTimer->setInterval(mAutoSaveDelay * 60000);
	// mAutoSaveTimer->setInterval(5000);
}

//incremental save will just save a new file each time...
void MainWindow::setIncrementalSave(int value)
{
	mIncrementalSave = (bool)value;
}

void MainWindow::setIncrementalSaveMax(double value)
{
	mIncrementalSaveMax = (int)value;
}

void MainWindow::setSaveDirectory(QString s){
	mSaveDirectory = s;
}

void MainWindow::checkSaveDirectory()
{
	QDir dir(mSaveDirectory);
	if (dir.exists())
	{
		// std::cout<<"good job\n";
	}
	else
	{
		QString s = QString(
			"Directory\n\n%1\n\ndoes not exist, "\
			"please type a different directory name.\n\n"\
			"Check to make sure there are no unnecessary spaces at the end.").arg(mSaveDirectory);
		QMessageBox::about(this, tr("TopMod Error"), s);
	}
}

void MainWindow::setCommandCompleterIndexToggle(int value)
{
	//stuff here...?
	// std::cout << "toggle command completer thing\n";
	mCommandCompleterIndexToggle = (bool)value;
}

//dave
void MainWindow::changeValence2SplitOffset(double value)
{
	MainWindow::sVertexSplitOffset = value;
}

void MainWindow::changeCornerCuttingAlpha(double value)
{
	MainWindow::sCornerCuttingAlpha = value;
}

void MainWindow::changeDomeExtrudeLength(double value)
{
  MainWindow::sDomeExtrudeLengthFactor = value;
}

void MainWindow::changeDooSabinExtrudeTwist(double value)
{
  MainWindow::sDooSabinExtrusionTwist = value;
}

void MainWindow::changeDomeExtrudeScale(double value)
{
  MainWindow::sDomeExtrudeScaleFactor = value;
}

void MainWindow::changeDomeExtrudeRotation(double value)
{
  MainWindow::sDomeExtrudeRotationFactor = value;
}

void MainWindow::setSingleClickExtrude(int value)
{
	mSingleClickExtrude = (bool)value;
}

// Selection Menu.
void MainWindow::select_vertex()
{
	setMode(MainWindow::SelectVertex);
}

void MainWindow::edit_vertex()
{
	setMode(MainWindow::EditVertex);
}

void MainWindow::select_face()
{
	setMode(MainWindow::SelectFace);
}

void MainWindow::select_face_loop()
{
	setMode(MainWindow::SelectFaceLoop);
}

void MainWindow::select_edge_ring()
{
	setMode(MainWindow::SelectEdgeRing);
}

void MainWindow::select_multiple_faces()
{
	setMode(MainWindow::MultiSelectFace);
}

void MainWindow::select_multiple()
{
	switch (mSelectionMask)
	{
	case MaskVertices:
		setMode(MainWindow::MultiSelectVertex);
		break;
	case MaskEdges:
		setMode(MainWindow::MultiSelectEdge);
		break;
	case MaskFaces://face stuff
		setMode(MainWindow::MultiSelectFace);
		break;
	case MaskCorners:
		break;
	default:
		break;
	}
}

void MainWindow::select_multiple_edges()
{
	setMode(MainWindow::MultiSelectEdge);
}

void MainWindow::select_multiple_vertices()
{
	setMode(MainWindow::MultiSelectVertex);
}

void MainWindow::select_similar_faces()
{
	// setMode(MainWindow::SelectSimilarFaces);
}

void MainWindow::select_similar()
{
	// switch(selectionmask){
	// 	case MaskFaces:
	setMode(MainWindow::SelectSimilar);
	// 	break;
	// 	case MaskEdges:
	// 	setMode(MainWindow::SelectSimilarEdges);
	// 	break;
	// 	case MaskVertices:
	// 	setMode(MainWindow::SelectSimilarVertices);
	// 	break;
	// 	default:
	// 	break;
	// };
}

void MainWindow::select_faces_by_area()
{
	setMode(MainWindow::SelectFacesByArea);
}

void MainWindow::select_faces_by_color()
{
	setMode(MainWindow::SelectFacesByColor);
}

void MainWindow::selection_window()
{
	setMode(MainWindow::SelectionWindow);
}

void MainWindow::select_checkerboard_faces()
{
	setMode(MainWindow::SelectCheckerboard);
}

void MainWindow::select_edge()
{
	setMode(MainWindow::SelectEdge);
}

void MainWindow::select_edge_loop()
{
	setMode(MainWindow::SelectEdgeLoop);
}

void MainWindow::select_corner()
{
	setMode(MainWindow::SelectCorner);
}

void MainWindow::selectionMaskVertices()
{
	// setSelectionMask(MainWindow::MaskVertices);
	setMode(MainWindow::SelectVertex);
}

void MainWindow::selectionMaskFaces()
{
	// setSelectionMask(MainWindow::MaskFaces);
	setMode(MainWindow::SelectFace);
}

void MainWindow::selectionMaskEdges()
{
	// setSelectionMask(MainWindow::MaskEdges);
	setMode(MainWindow::SelectEdge);
}

void MainWindow::selectionMaskCorners()
{
	// setSelectionMask(MainWindow::MaskCorners);
	setMode(MainWindow::SelectCorner);
}

void MainWindow::selectAll()
{
	switch (mSelectionMask)
	{
	case MaskVertices:
		mActiveViewport->selectAllVertices();
		break;
	case MaskEdges:
		mActiveViewport->selectAllEdges();
		break;
	case MaskFaces:
		mActiveViewport->selectAllFaces();
		break;
	case MaskCorners:
		// active->selectAllCorners();
	case MaskObject:
		break;
	default:
		// active->selectAllVertices();
		break;
	}
}

void MainWindow::selectInverse()
{
	switch (mSelectionMask)
	{
	case MaskVertices:
		mActiveViewport->selectInverseVertices();
		break;
	case MaskEdges:
		mActiveViewport->selectInverseEdges();
		break;
	case MaskFaces:
		mActiveViewport->selectInverseFaces();
		break;
	case MaskCorners:
		mActiveViewport->selectInverseFaceVertices();
	case MaskObject:
		break;
	default:
	// active->selectInverseVertices();
		break;
	}
}

void MainWindow::exit_selection_mode()
{
	setMode(MainWindow::NormalMode);
	redraw();
}

void MainWindow::clear_selected()
{
	MainWindow::clearSelected();
	redraw();
}

//dave
void MainWindow::toggleWireframeSplit(int state)
{
	MainWindow::sWireframeSplit = bool(state);
}

// Basics.
void MainWindow::toggleDeleteEdgeCleanupFlag(int state)
{
	MainWindow::sDeleteEdgeCleanup = bool(state);
}

void MainWindow::changeNumSubDivs(double value)
{
	MainWindow::sNumEdgeSubdiv = (int)value;
}

void MainWindow::changeFaceAreaTolerance(double value)
{
	MainWindow::sFaceAreaTolerance = value;
}

void MainWindow::changeTileTexNum(double value)
{
	MainWindow::sTileTexSize = (int)value;
}

// Extrusions.
void MainWindow::changeExtrudeLength(double value)
{
	MainWindow::sExtrudeDist = value;
}

void MainWindow::changeExtrudeRotation(double value)
{
	MainWindow::sExtrudeRot = (int)value;
}

void MainWindow::changeExtrudeScale(double value)
{
	MainWindow::sExtrudeScale = value;
}

void MainWindow::changeExtrudeLength1(double value)
{
	MainWindow::sExtrudeLength1 = value;
}

void MainWindow::changeExtrudeLength2(double value)
{
	MainWindow::sExtrudeLength2 = value;
}

void MainWindow::changeExtrudeLength3(double value)
{
	MainWindow::sExtrudeLength3 = value;
}

void MainWindow::changeExtrudeAngle(double value)
{
	MainWindow::sExtrudeAngle = value;
}

void MainWindow::changeExtrudeLength1Icosa(double value)
{
	MainWindow::sExtrudeLength1_iCosA = value;
}

void MainWindow::changeExtrudeLength2Icosa(double value)
{
	MainWindow::sExtrudeLength2_iCosA = value;
}

void MainWindow::changeExtrudeLength3Icosa(double value)
{
	MainWindow::sExtrudeLength3_iCosA = value;
}

void MainWindow::changeExtrudeAngleIcosa(double value)
{
	MainWindow::sExtrudeAngle_iCosA = value;
}

void MainWindow::changeNumExtrusions(double value)
{
	MainWindow::sNumExtrusions = (int)value;
}

void MainWindow::toggleDualMeshEdgesFlag(int state)
{
	MainWindow::sDualMeshEdgesCheck = bool(state);
}

void MainWindow::toggleHexagonalizeDodecaExtrudeFlag(int state)
{
	MainWindow::sHexagonalizeDodecaExtrude = bool(state);
}

// Stellation
void MainWindow::changeStellateLength(double value)
{
	MainWindow::sExtrudeDist = value;
}

// Holes and Handles.
void MainWindow::changeMaxSegments(double value)
{
	MainWindow::sMaxSegments = (int)value;
}

void MainWindow::changeNumSegments(double value)
{
	MainWindow::sNumSegments = (int)value;
}

void MainWindow::changeNumSegments2(double value)
{

}

void MainWindow::changeMaxSegments2(double value)
{

}

void MainWindow::changeWeight1(double value)
{
	MainWindow::sFaceNormalWeight1 = value;
}

void MainWindow::toggleSymmetricWeightsFlag(int state)
{
	MainWindow::sSymmetricWeights = bool(state);
}

void MainWindow::changeWeight2(double value)
{
	MainWindow::sFaceNormalWeight2 = value;
}

void MainWindow::changeExtraTwists(double value)
{
	MainWindow::sNumExtraTwists = (int)value;
}

//added by Ryan
void MainWindow::changePinch(double value)
{
	MainWindow::sPinchingFactor = value;
}
void MainWindow::changePinchCenter(double value)
{
	MainWindow::sPinchCenter = value;
}
void MainWindow::changeBubble(double value)
{
	MainWindow::sBubbleFactor = value;
}
void MainWindow::changeHoleHandlePinchValue(double value)
{
	MainWindow::sHoleHandlePinchingFactor = value;
}
void MainWindow::changeHoleHandlePinchCenterValue(double value)
{
	MainWindow::sHoleHandlePinchCenter = value;
}
void MainWindow::changeHoleHandlePinchWidthValue(double value)
{
	MainWindow::sHoleHandlePinchWidth = value;
}
//end Ryan's stuff

// Crust Modeling.
void MainWindow::changeCrustScaleFactor(double value)
{
	MainWindow::sCrustScaleFactor = value;
}

void MainWindow::changeCrustThickness(double value)
{
	MainWindow::sCrustThickness = value;
}

void MainWindow::toggleCrustCleanupFlag(int state)
{
	MainWindow::sCrustCleanup = bool(state);
}

void MainWindow::crustModeling1()
{
	setMode(MainWindow::CrustModeling);
	createCrust(true);
	redraw();
}

void MainWindow::crustModeling2()
{
	setMode(MainWindow::CrustModeling);
	createCrust(false);
	redraw();
}

void MainWindow::crustModeling3()
{
	// setMode(MainWindow::CrustModelingPainting);
	createCrust2(false);
	redraw();
}

void MainWindow::crustModeling4()
{
	// setMode(MainWindow::CrustModelingPainting);
	createCrust2(true);
	redraw();
}

//for modified corner cutting scheme... it's the same as the first step in the wireframe process
void MainWindow::changeModifiedCornerCuttingThickness(double value)
{
	MainWindow::sModifiedCornerCuttingThickness = value;
}

//column modeling and wireframe modeling
void MainWindow::changeWireframeThickness(double value)
{
	MainWindow::sWireframeThickness = value;
}

void MainWindow::changeWireframe2Thickness(double value)
{
	MainWindow::sWireframe2Thickness = value;
}

void MainWindow::changeWireframe2Width(double value)
{
	MainWindow::sWireframe2Width = value;
}

void MainWindow::changeColumnThickness(double value)
{
	MainWindow::sColumnThickness = value;
}

void MainWindow::changeColumnSegments(double value)
{
	MainWindow::sColumnSegments = (int)value;
}

// Multi-face handle
void MainWindow::mfh_use_closest_edge_algo()
{
	MainWindow::sMfhAlgorithm = MainWindow::ClosestEdge;
}

void MainWindow::mfh_use_convex_hull_algo()
{
	MainWindow::sMfhAlgorithm = MainWindow::ConvexHull;
}

void MainWindow::changeMultiFaceHandleScaleFactor(double value)
{
	MainWindow::sMfhScaleFactor = value;
}

void MainWindow::changeMultiFaceHandleExtrudeDist(double value)
{
	MainWindow::sMfhExtrudeDist = value;
}

void MainWindow::toggleMultiFaceHandleUseMaxOffsetFlag(int state)
{
	MainWindow::sMfhUseMaxOffsets = bool(state);
}

//menger sponge operations
void MainWindow::changeMengerSpongeThickness(double value)
{
	MainWindow::sSpongeThickness = value;
}

void MainWindow::changeMengerSpongeCollapseThreshold(double value)
{
	MainWindow::sSpongeCollapseThreshold = value;
}

void MainWindow::toggleMengerSpongeFractionalThicknessFlag(int state)
{
	MainWindow::sSpongeFractionalThickness = bool(state);
}

/* begin ozgur */
//sculpting operations from ozgur
//conical stuff from ozgur
void MainWindow::performCutting()
{
	// DLFLEdgePtrArray eparray;
	// DLFLVertexPtrArray vparray;
	// DLFLEdgePtrArray fparray;

	undoPush();
	setModified(true);
	// void performCutting( DLFLObjectPtr obj, int type,float offsetE,float offsetV,bool global,bool selected) {
	// DLFL::performCutting(&object, MainWindow::mode,MainWindow::cutOffsetE_factor,MainWindow::cutOffsetV_factor,MainWindow::global_cut,MainWindow::selected_cut);
	switch (mSelectionMask)
	{
	case MaskEdges:
	// std::cout << "cut by edge!\n";
	// active->getSelectedEdges(eparray);
		DLFL::cutSelectedEdges(&mObject, MainWindow::sCutOffsetEdgeFactor, MainWindow::sCutOffsetVertFactor, MainWindow::sGlobalCut);
		break;
	case MaskVertices:
	// std::cout << "cut by vertex!\n";
	// active->getSelectedVertices(vparray);
		DLFL::cutSelectedVertices(&mObject, MainWindow::sCutOffsetEdgeFactor, MainWindow::sCutOffsetVertFactor, MainWindow::sGlobalCut);
		break;
	case MaskFaces:
	// std::cout << "cut by face!\n";
	// active->getSelectedFaces(fparray);
		DLFL::cutSelectedFaces(&mObject, MainWindow::sCutOffsetEdgeFactor, MainWindow::sCutOffsetVertFactor, MainWindow::sGlobalCut);
		break;
	default:
		break;
	}
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createConvexHull()
{
	undoPush();
	setModified(true);
	DLFL::createConvexHull(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createDualConvexHull()
{
	undoPush();
	setModified(true);
	DLFL::createDualConvexHull(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::changeCutOffsetE(double value)
{
	MainWindow::sCutOffsetEdgeFactor = value;
}

void MainWindow::changeCutOffsetV(double value)
{
	MainWindow::sCutOffsetVertFactor = value;
}

void MainWindow::changePNormalBendS(double value)
{
	MainWindow::sPNormalBendS_Factor = value;
}

void MainWindow::changePNormalBendT(double value)
{
	MainWindow::sPNormalBendT_Factor = value;
}

void MainWindow::toggleGlobalCut(int state)//ozgur
{
	MainWindow::sGlobalCut = bool(state);
}
void MainWindow::toggleSelectedCut(int state)//ozgur
{
	MainWindow::sSelectedCut = bool(state);
}

void MainWindow::changeTiltPlane1(double value)
{
}
void MainWindow::changeTiltPlane2(double value)
{
}
/* end ozgur */

//dave 11.07
void MainWindow::setPaintBucketColor(QColor c)
{
	MainWindow::sPaintBucketColor = c;
}

void MainWindow::paintSelectedFaces()
{
	//iterate through the selected faces and color them with object.addMaterial?
	undoPush();
	// DLFLFacePtrArray fparray;
	// fparray.resize(active->numSelectedFaces());
	for (int i = 0; i < mActiveViewport->numSelectedFaces(); ++i)
	{
		mActiveViewport->getSelectedFace(i)->setMaterial(
			mObject.addMaterial(
				RGBColor(sPaintBucketColor.redF(), sPaintBucketColor.greenF(), sPaintBucketColor.blueF())));
	}
	MainWindow::clearSelected();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

void MainWindow::clearMaterials()
{
	undoPush();
	mObject.clearMaterials();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

// Geometric transformations
void MainWindow::translatex(double x)
{
	Vector3d oldtr = mObject.position;
	mObject.position[0] = x;
	mObject.tr.transform[0][3] = x;
	redraw();
}

void MainWindow::translatey(double y)
{
	mObject.position[1] = y;
	mObject.tr.transform[1][3] = y;
	redraw();
}

void MainWindow::translatez(double z)
{
	mObject.position[2] = z;
	mObject.tr.transform[2][3] = z;
	redraw();
}

void MainWindow::scalex(double x)
{
	mObject.scale_factor[0] = x;
	mObject.tr.transform[0][0] = x;
	redraw();
}

void MainWindow::scaley(double y)
{
	mObject.scale_factor[1] = y;
	mObject.tr.transform[1][1] = y;
	redraw();
}

void MainWindow::scalez(double z)
{
	mObject.scale_factor[2] = z;
	mObject.tr.transform[2][2] = z;
	redraw();
}

void MainWindow::freezeTransforms()
{
	mObject.freezeTransformations();
	mObject.position.reset();
	mObject.scale_factor.set(1, 1, 1);
	mObject.rotation.reset();
	mObject.tr.reset();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeLighting();
	redraw();
}

/*// Global operations (don't require selection)
void MainWindow::active->recomputeNormals(void)     // Recompute normals and lighting
{
	object.computeNormals();
	computeLighting( &object, patchObject, &plight);
}

void MainWindow::recomputeLighting(void)                // Recompute lighting
{
	computeLighting( &object, patchObject, &plight);
}

void MainWindow::active->recomputePatches(void) // Recompute the patches for patch rendering
{
	if(patchObject)
		patchObject->updatePatches(&object);
}*/

// Sub-divide all edges
void MainWindow::subdivideAllEdges(void)
{
	undoPush();
	DLFL::subdivideAllEdges(&mObject, MainWindow::sNumEdgeSubdiv);
	MainWindow::clearSelected();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

void MainWindow::subdivideSelectedEdges(void) // Subdivide all selected edges
{
	undoPush();
	// DLFLEdgePtrArray eparray;
	// eparray.resize(active->numSelectedEdges());
	for (int i=0; i < mActiveViewport->numSelectedEdges(); ++i)	{
		subdivideEdge(&mObject,MainWindow::sNumEdgeSubdiv, mActiveViewport->getSelectedEdge(i));
	}
	// DLFL::subdivideEdges(&object,fparray,use_quads);
	MainWindow::clearSelected();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

void MainWindow::subdivideSelectedFaces(void) // Subdivide all selected faces
{
	undoPush();
	DLFLFacePtrArray fparray;
	fparray.resize(mActiveViewport->numSelectedFaces());
	for (int i = 0; i < mActiveViewport->numSelectedFaces(); ++i)
	{
		fparray[i] = mActiveViewport->getSelectedFace(i);
	}
	DLFL::subdivideFaces(&mObject, fparray, sUseQuads);
	MainWindow::clearSelected();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

void MainWindow::subdivideAllFaces(void) // Subdivide all the faces
{
	undoPush();
	DLFL::subdivideAllFaces(&mObject,sUseQuads);
	MainWindow::clearSelected();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

void MainWindow::createMultiFaceHandle(void) // Create multi-face handle between selected faces
{
	int numsel = mActiveViewport->numSelectedFaces();
	if (numsel < 3) return;
	undoPush();
	DLFLFacePtrArray sel_faces;
	DLFLFacePtr sfptr;
	for (int i = 0; i < numsel; ++i)
	{
		sfptr = mActiveViewport->getSelectedFace(i);
		if (sfptr) sel_faces.push_back(sfptr);
	}
	switch (MainWindow::sMfhAlgorithm)
	{
	case ConvexHull:
		DLFL::multiConnectFaces(&mObject, sel_faces, MainWindow::sMfhScaleFactor, MainWindow::sMfhExtrudeDist, MainWindow::sMfhUseMaxOffsets);
		break;
	case ClosestEdge:
		DLFL::multiConnectFaces(&mObject, sel_faces);
		break;
	default:
		break;
	}
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::multiConnectMidpoints(void)
{
	// Multi-connect midpoints after simplest-subdivision without edge deletion
	undoPush();
	DLFL::multiConnectMidpoints(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::multiConnectCrust(void)
{
	// Multi-connect after creating crust
	undoPush();
	DLFL::multiConnectCrust(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::modifiedMultiConnectCrust(void)
{
	// Modified multi-connect after creating crust
	undoPush();
	DLFL::modifiedMultiConnectCrust(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

void MainWindow::createSponge(void)
{
	undoPush();
	DLFL::createSponge(&mObject,MainWindow::sSpongeThickness,
		MainWindow::sSpongeCollapseThreshold);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

// Planarize all faces
void MainWindow::planarizeFaces(void)
{
	undoPush();
	DLFL::planarize(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

// Spheralize object vertices
void MainWindow::spheralizeObject(void)
{
	undoPush();
	DLFL::spheralize(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

// Smooth the mesh
void MainWindow::smoothMesh(void)
{
	undoPush();
	DLFL::meshsmooth(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	redraw();
}

// Catmull-Clark subdivision
void MainWindow::subdivideCatmullClark(void)
{
	undoPush();
	DLFL::catmullClarkSubdivide(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd( "subdivide(\"catmull-clark\")");
	emit echoCommand( cmd );
	// redraw();
}

// Doo-Sabin subdivision
void MainWindow::subdivideDooSabin(void)
{
	undoPush();

	// QProgressDialog *progress = new QProgressDialog("Performing Doo Sabin Remeshing...", "Cancel", 0, 1);
	// progress->setMinimumDuration(2000);
	// progress->setWindowModality(Qt::WindowModal);

	// DLFLObjectPtr t = object;
	// active->setRenderingEnabled(false);

	// if (!DLFL::dooSabinSubdivide(&object, doo_sabin_check/* , progress*/) ){
	// active->recomputePatches();
	// active->recomputeNormals();
	// undo();
	// MainWindow::clearSelected();
	// active->setRenderingEnabled(true);
	// }
	// else {
	// object = t;
	// delete t;
	// active->setRenderingEnabled(true);
	DLFL::dooSabinSubdivide(&mObject, sDooSabinCheck/* , progress*/);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"doo-sabin\",");
	QString check("False");
	if (sDooSabinCheck)
		check = QString("True");
	cmd += check + QString(")");
	emit echoCommand(cmd);
	// }

}

// Honeycomb subdivision
void MainWindow::subdivideHoneycomb(void)
{
	undoPush();
	DLFL::honeycombSubdivide(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"honeycomb\")");
	emit echoCommand(cmd);
}

// Root-4 subdivision
void MainWindow::subdivideRoot4(void)
{
	undoPush();
	DLFL::root4Subdivide(&mObject, MainWindow::sWeightFactor, MainWindow::sTwistFactor);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"root4\",");
	cmd += QString().setNum(MainWindow::sWeightFactor) + QString(",");
	cmd += QString().setNum(MainWindow::sTwistFactor) + QString(")");
	emit echoCommand(cmd);
}

// Corner-cutting subdivision
void MainWindow::subdivideCornerCutting(void)
{
	undoPush();
	DLFL::cornerCuttingSubdivide(&mObject, MainWindow::sCornerCuttingAlpha);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"corner-cut\")");
	emit echoCommand(cmd);
}

// Modified Corner-cutting subdivision
void MainWindow::subdivideModifiedCornerCutting(void)
{
	undoPush();
	DLFL::modifiedCornerCuttingSubdivide(&mObject, MainWindow::sModifiedCornerCuttingThickness);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	// QString cmd("subdivide(\"modified-corner-cut\")");
	// emit echoCommand( cmd );
}

// Bi-linear Vertex-insertion remeshing
void MainWindow::subdivideLinearVertexInsertion(void)
{
	undoPush();
	DLFL::subdivideAllFaces(&mObject, true);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"linear-vertex\",True)");
	emit echoCommand(cmd);
}

// Corner-cutting subdivision
void MainWindow::subdivideSimplest(void)
{
	undoPush();
	DLFL::simplestSubdivide(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"simplest\")");
	emit echoCommand(cmd);
}

// Vertex-cutting subdivision
void MainWindow::subdivideVertexCutting(void)
{
	undoPush();
	DLFL::vertexCuttingSubdivide(&mObject, MainWindow::sVertexCuttingOffset);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"vertex-cut\",")
		+ QString().setNum(MainWindow::sVertexCuttingOffset)
		+ QString(")");
	emit echoCommand(cmd);
}

// Pentagonal subdivision
void MainWindow::subdividePentagonal(void)
{
	undoPush();
	DLFL::pentagonalSubdivide(&mObject, MainWindow::sPentagonalOffset);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"pentagon\",");
	cmd += QString().setNum(MainWindow::sPentagonalOffset) + QString(")");
	emit echoCommand(cmd);
}

// Cubic Pentagonal remeshing scheme
void MainWindow::subdivideCubicPentagonal(void)
{
// Implemented as Pentagonal + Dual + Dual
	undoPush();
	DLFL::pentagonalSubdivide(&mObject, MainWindow::sPentagonalOffset);
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"pentagon\",");
	cmd += QString().setNum(MainWindow::sPentagonalOffset) + QString(")");
	cmd += QString("\ndual()\ndual()");
	emit echoCommand(cmd);
}

// Dual-Pentagonal subdivision
void MainWindow::subdivideDualPentagonal(void)
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::pentagonalSubdivide(&mObject, MainWindow::sPentagonalOffset);
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("dual()\nsubdivide(\"pentagon\",");
	cmd += QString().setNum(MainWindow::sPentagonalOffset) + QString(")");
	cmd += QString("\ndual()");
	emit echoCommand(cmd);
}

// Pentagon preserving remeshing
void MainWindow::subdividePentagonPreserving(void)
{
	undoPush();
	DLFL::pentagonalSubdivide2(&mObject, MainWindow::sPentagonalScale);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"pentagon-preserve\",");
	cmd += QString().setNum(MainWindow::sPentagonalScale) + QString(")");
	emit echoCommand(cmd);
}

// Dual Pentagon preserving remeshing
void MainWindow::subdivideDualPentagonPreserving(void)
{
// Implemented as Dual + Pentagonal + Dual
	undoPush();
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::pentagonalSubdivide2(&mObject, MainWindow::sPentagonalScale);
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("dual()\nsubdivide(\"pentagon-preserve\",");
	cmd += QString().setNum(MainWindow::sPentagonalScale) + QString(")");
	cmd += QString("\ndual()");
	emit echoCommand(cmd);
}

// Dual Hexagon Preserving remeshing
void MainWindow::subdivideDualHexagonPreserving(void)
{
// Implemented as Dual + Root4 + Dual
	undoPush();
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::root4Subdivide(&mObject, MainWindow::sWeightFactor, MainWindow::sTwistFactor);
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("dual()\nsubdivide(\"root4\",");
	cmd += QString().setNum(MainWindow::sWeightFactor) + QString(",");
	cmd += QString().setNum(MainWindow::sTwistFactor) + QString(")");
	cmd += QString("\ndual()");
	emit echoCommand(cmd);
}

// Root-3 remeshing
void MainWindow::subdivideRoot3(void)
{
// Implemented as Dual + Honeycomb + Dual
	undoPush();
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::honeycombSubdivide(&mObject);
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()\n");
	cmd += QString("subdivide(\"honeycomb\")\n");
	cmd += QString("dual()");
	emit echoCommand(cmd);
}

// Loop subdivision
void MainWindow::subdivideLoop(void)
{
	undoPush();
	DLFL::loopSubdivide(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"loop\")\n");
	emit echoCommand(cmd);
}

// Dual of Loop subdivision
void MainWindow::subdivideDualLoop(void)
{
	// Implemented as Dual + Loop + Dual
	undoPush();
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::loopSubdivide(&mObject);
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()\n");
	cmd += QString("subdivide(\"loop\")\n");
	cmd += QString("dual()");
	emit echoCommand(cmd);
}

// 12-6-4 remeshing
void MainWindow::subdivide1264(void)
{
	// Implemented as Dual + Dual 12-6-4 + Dual
	undoPush();
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::dual1264Subdivide(&mObject, MainWindow::sDual1264ScaleFactor);
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()\ndual()\n");
	cmd += QString("subdivide(\"dual-12.6.4\",")
		+ QString().setNum(MainWindow::sDual1264ScaleFactor)
		+ QString(")\n");
	cmd += QString("dual()");
	emit echoCommand(cmd);
}

// Dual of 12-6-4 remeshing - Bei & Cansin
void MainWindow::subdivideDual1264(void)
{
	undoPush();
	DLFL::dual1264Subdivide(&mObject, MainWindow::sDual1264ScaleFactor);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"dual-12.6.4\",");
	cmd += QString().setNum(MainWindow::sDual1264ScaleFactor);
	cmd += QString(")");
	emit echoCommand(cmd);
}

// Checker board remeshing
void MainWindow::subdivideCheckerBoard(void)
{
	undoPush();
	DLFL::checkerBoardRemeshing(&mObject, MainWindow::sCheckerboardThickness);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("subdivide(\"checker\",");
	cmd += QString().setNum(MainWindow::sCheckerboardThickness);
	cmd += QString(")");
	emit echoCommand(cmd);
}

// Dual Checker board remeshing
void MainWindow::subdivideDualCheckerBoard(void)
{
	// Implemented as Dual + Checker board + Dual
	undoPush();
	setModified(true);
	DLFL::createDual(&mObject, true); // Use accurate method
	DLFL::checkerBoardRemeshing(&mObject, MainWindow::sCheckerboardThickness);
	DLFL::createDual(&mObject, true); // Use accurate method
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd("dual()\nsubdivide(\"checker\",");
	cmd += QString().setNum(MainWindow::sCheckerboardThickness);
	cmd += QString(")\ndual()");
	emit echoCommand(cmd);
}

// Star subdivision - Doug
void MainWindow::subdivideStar(void)
{
	undoPush();
	setModified(true);
	DLFL::starSubdivide(&mObject, MainWindow::sStarOffset);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"star\")");
	emit echoCommand(cmd);
}

// sqrt(3) subdivision - Doug
void MainWindow::subdivideSqrt3(void)
{
	undoPush();
	setModified(true);
	DLFL::sqrt3Subdivide(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"sqrt3\")");
	emit echoCommand(cmd);
}

// fractal - Doug
void MainWindow::subdivideFractal(void)
{
	undoPush();
	setModified(true);
	DLFL::fractalSubdivide(&mObject, MainWindow::sFractalOffset);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd("subdivide(\"fractal\",");
	cmd += QString().setNum(MainWindow::sFractalOffset);
	cmd += QString(")");
	emit echoCommand(cmd);
}

// stellate subdivision - Eric
void MainWindow::subStellate1(void)
{
	undoPush();
	setModified(true);
	DLFL::stellateSubdivide(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"stellate\")");
	emit echoCommand(cmd);
}

// stellate subdivision - Eric
void MainWindow::subStellate2(void)
{
	undoPush();
	setModified(true);
	DLFL::twostellateSubdivide(&mObject, MainWindow::sSubstellateHeight, MainWindow::sSubstellateCurve);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"double-stellate\",");
	cmd += QString().setNum(MainWindow::sSubstellateHeight) + QString(",");
	cmd += QString().setNum(MainWindow::sSubstellateCurve) + QString(")");
	emit echoCommand(cmd);
}

// Bei & Cansin
void MainWindow::subdivideDome(void)
{
	undoPush();
	setModified(true);
	DLFL::domeSubdivide(&mObject, MainWindow::sDomeLengthFactor, MainWindow::sDomeScaleFactor);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"dome\",");
	cmd += QString().setNum(MainWindow::sSubstellateHeight) + QString(",");
	cmd += QString().setNum(MainWindow::sSubstellateCurve) + QString(")");
	emit echoCommand(cmd);
}

// Doo-Sabin(BC) subdivision - Bei & Cansin
void MainWindow::subdivideDooSabinBC(void)
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBC(&mObject, MainWindow::sDooSabinCheck);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString check("False");
	if (MainWindow::sDooSabinCheck)
		check = QString("True");
	QString cmd = QString("subdivide(\"doo-sabin-bc\",");
	cmd += check + QString(")");
	emit echoCommand(cmd);
}

// Doo-Sabin(BCNew) Bei & Cansin
void MainWindow::subdivideDooSabinBCNew(void)
{
	undoPush();
	setModified(true);
	DLFL::dooSabinSubdivideBCNew(&mObject, MainWindow::sDooSabinBCnewScaleFactor,
								 MainWindow::sDooSabinBCnewLengthFactor);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd = QString("subdivide(\"doo-sabin-bc-new\",");
	cmd += QString().setNum(MainWindow::sDooSabinBCnewScaleFactor) + QString(",");
	cmd += QString().setNum(MainWindow::sDooSabinBCnewLengthFactor) + QString(")");
	emit echoCommand(cmd);
}

// Loop-style subdivision - Bei
void MainWindow::subdivideLoopStyle(void)
{
	undoPush();
	setModified(true);
	DLFL::loopStyleSubdivide(&mObject, MainWindow::sLoopLengthFactor);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	QString cmd = QString("subdivide(\"loop-style\",");
	cmd += QString().setNum(MainWindow::sLoopLengthFactor) + QString(")");
	emit echoCommand(cmd);
}

void MainWindow::globalStellate(void)
{
	// Does not use length parameter for now. Uses subdivideFace method with triangles
	undoPush();
	setModified(true);
	DLFL::subdivideAllFaces(&mObject, false);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("subdivide(\"allfaces\",False)");
	emit echoCommand(cmd);
}

void MainWindow::triangulate(void)
{
	undoPush();
	setModified(true);
	DLFL::triangulateAllFaces(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	// QString cmd = QString("subdivide(\"triangulate\",False)");
	// emit echoCommand( cmd );
}

// Split Valence 2 vertices
void MainWindow::splitValence2Vertices(void)
{
	undoPush();
	setModified(true);
	DLFL::splitValence2Vertices(&mObject, MainWindow::sVertexSplitOffset);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

// cleanup 2-gons
void MainWindow::cleanup2gons(void)
{
	undoPush();
	setModified(true);
	DLFL::cleanup2gons(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

// Remove valence 2 vertices
void MainWindow::cleanupWingedVertices(void)
{
	undoPush();
	setModified(true);
	DLFL::cleanupWingedVertices(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

// Create dual object
void MainWindow::createDual(void)
{
	undoPush();
	setModified(true);
	DLFL::createDual(&mObject, MainWindow::sAccurateDual);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();

	QString cmd = QString("dual()");
	emit echoCommand(cmd);
}

// Create a crust
void MainWindow::createCrust(bool use_scaling)
{
	undoPush();
	setModified(true);
	if (use_scaling)
		DLFL::createCrustWithScaling(&mObject, MainWindow::sCrustScaleFactor);
	else
		DLFL::createCrust(&mObject, MainWindow::sCrustThickness);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

//this function will tag all currently selected faces with the FTHole type
//when the user presses the rind modeling button, and it will punch out all those faces
void MainWindow::createCrust2(bool use_scaling)
{
	QString facelist("[");

	undoPush();
	setModified(true);
	if (use_scaling)
		DLFL::createCrustWithScaling(&mObject, MainWindow::sCrustScaleFactor);
	else
		DLFL::createCrust(&mObject, MainWindow::sCrustThickness);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	if (mActiveViewport->numSelectedFaces() >= 1)
	{
		DLFLFacePtrArray sfptrarr = mActiveViewport->getSelectedFaces();
		if (sfptrarr[0])
		{
			for (auto sfPtr : sfptrarr)
			{
				sfPtr->setType(FTHole);
				facelist += QString().setNum(sfPtr->getID()) + QString(",");
			}

			facelist += QString("]");
			DLFL::punchHoles(&mObject);
		}
	}
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->clearSelectedFaces();
	redraw();

	QString cmd = QString("rind(") +
		facelist + QString(",True,") +
		MainWindow::sCrustThickness +
		QString(")");
	emit echoCommand(cmd);
}

// Create a wireframe
void MainWindow::makeWireframe(void)
{
	undoPush();
	setModified(true);
	DLFL::makeWireframe(&mObject, MainWindow::sWireframeThickness, MainWindow::sWireframeSplit);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

// Create a wireframe // dave
void MainWindow::makeWireframe2()
{
	undoPush();
	setModified(true);
	DLFL::makeWireframe2(
		&mObject,
		MainWindow::sWireframe2Thickness,
		MainWindow::sWireframe2Width,
		MainWindow::sWireframeSplit);
	// active->recomputePatches();
	// active->recomputeNormals();
	// if ( active->numSelectedFaces() >= 1 ) {
	// 	DLFLFacePtrArray sfptrarr = active->getSelectedFaces();
	// 	if ( sfptrarr[0] ) {
	// 		//mark all the faces for hole punching
	// 		for(auto sfPtr : sfptrarr)
	// 			sfPtr->setType(FTHole);
	// 		// object.punchHoles();
	// 		object.makeWireframe2(MainWindow::wireframe_thickness);
	// 	}
	// }
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

// Create a wireframe using columns
void MainWindow::makeWireframeWithColumns(void)
{
	undoPush();
	setModified(true);
	DLFL::makeWireframeWithColumns(&mObject, MainWindow::sColumnThickness, MainWindow::sColumnSegments);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

/*
void MainWindow::makeUnitCube(double edgelength)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr unitcube = DLFLObject::makeUnitCube(edgelength);
	object.reset();
	object.splice(*unitcube);
	delete unitcube;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::makeUnitTetrahedron(double edgelength)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr unittetra = DLFLObject::makeUnitTetrahedron();
	object.reset();
	object.splice(*unittetra);
	delete unittetra;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::makeMengerSponge(int level)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr mengersponge = DLFLObject::makeMengerSponge(level);
	object.reset();
	object.splice(*mengersponge);
	delete mengersponge;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}

void MainWindow::makeSierpinskiTetrahedron(int level)
{
	undoPush();
	setModified(true);
	DLFLObjectPtr stetra = DLFLObject::makeSierpinskiTetrahedron(level);
	object.reset();
	object.splice(*stetra);
	delete stetra;
	active->recomputePatches();
	active->recomputeNormals();
	MainWindow::clearSelected();
}
*/

// Randomly assign texture coordinates
void MainWindow::randomAssignTexCoords(void)
{
	mObject.randomAssignTexCoords();
	redraw();
}

// Assign texture coordinates for tile texturing
void MainWindow::assignTileTexCoords(void)
{
	mObject.assignTileTexCoords(MainWindow::sTileTexSize);
	redraw();
}

// Cleanup redundant edges
void MainWindow::edgeCleanup(void)
{
	undoPush();
	setModified(true);
	DLFL::edgeCleanup(&mObject);
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	MainWindow::clearSelected();
	redraw();
}

// Print summary information
void MainWindow::printSummary(void)
{
	mObject.print();
}

// Print vertex list
void MainWindow::printVertexList(void)
{
	mObject.printVertexList();
}

// Print edge list
void MainWindow::printEdgeList(void)
{
	mObject.printEdgeList();
}

// Print CV list
void MainWindow::printCVList(void)
{
	///object.printPatchCVList();
}

// Print face list
void MainWindow::printFaceList(void)
{
	mObject.printFaces();
}

void MainWindow::getCommand()
{
#ifdef QCOMPLETER
	int i = mCommandCompleter->exec();
	mCommandCompleter->setFocus(Qt::PopupFocusReason);
	// QMessageBox::about(this, tr("%1").arg(text),tr("%1").arg(mCommandList.indexOf(text)));
	if (i > -1)
		mActionListWidget->actions().at(i)->activate(QAction::Trigger);
#endif
}

void MainWindow::initializeAnimatedHelp()
{
	mAnimatedHelpWidget = new QWidget(this);
	// QSplashScreen *w = new QSplashScreen(this);
	mAnimatedHelpLayout = new QVBoxLayout;

	mAnimatedHelpMovie = new QMovie(this);
	mAnimatedHelpMovie->setCacheMode(QMovie::CacheAll);
	mAnimatedHelpMovie->setBackgroundColor(QColor(255, 255, 255, 255));

	mAnimatedHelpLabel = new QLabel(tr("No movie loaded"));
	mAnimatedHelpLabel->setAlignment(Qt::AlignCenter);
	// mAnimatedHelpLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	// mAnimatedHelpLabel->setBackgroundRole(QPalette::Dark);
	// mAnimatedHelpLabel->setAutoFillBackground(true);

	mAnimatedHelpLayout->addWidget(mAnimatedHelpLabel);
	mAnimatedHelpLayout->addStretch(1);
	mAnimatedHelpLayout->setMargin(0);
	mAnimatedHelpWidget->setLayout(mAnimatedHelpLayout);

	mAnimatedHelpMovie->stop();
	mAnimatedHelpLabel->setMovie(mAnimatedHelpMovie);
	// mAnimatedHelp->setFileName("images/insert_edge.mng");
	// w->move(QCursor::pos());
	// w->show();

	// create the dockwidget, set it to the right side
	mAnimatedHelpDockWidget = new QDockWidget(tr("TopMod Animated Help"), this);
	mAnimatedHelpDockWidget->setAllowedAreas(Qt::NoDockWidgetArea);
	mAnimatedHelpDockWidget->setWidget(mAnimatedHelpWidget);
	// addDockWidget(Qt::RightDockWidgetArea, mAnimatedHelpDockWidget);
	mAnimatedHelpDockWidget->hide();
	mAnimatedHelpDockWidget->setFloating(true);
	mAnimatedHelpDockWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
	// mAnimatedHelpDockWidget->setResizeEnabled(false);
	//sizing
	mAnimatedHelpDockWidget->setMinimumSize(200, 200);
	mAnimatedHelpDockWidget->setMaximumSize(200, 200);

	//fix positioning later
	mAnimatedHelpDockWidget->move(width(), 150 + mToolOptionsDockWidget->height() + 200);
}

void MainWindow::setAnimatedHelpImage()
{
	mAnimatedHelpMovie->setFileName("images/insert_edge.mng");
	mAnimatedHelpMovie->start();
}

void MainWindow::deleteSelected()
{
	DLFLEdgePtrArray septrarr;
	DLFLFacePtrArray sfptrarr;
	DLFLVertexPtrArray svptrarr;
	DLFLFaceVertexPtrArray sfvptrarr;

	// undoPush();
	// setModified(true);
	switch (mSelectionMask)
	{
	case MaskEdges:
	{
		septrarr = mActiveViewport->getSelectedEdges();
		if (septrarr[0])
		{
			undoPush();
			setModified(true);
			for (auto sePtr : septrarr)
			{
				DLFL::deleteEdge(&mObject, sePtr, true);
				// DLFL::deleteEdge( &object, septr, MainWindow::delete_edge_cleanup);
				// active->recomputePatches();
				// active->recomputeNormals();
			}
		}
		mActiveViewport->clearSelectedEdges();
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
		redraw();
		break;
	}
	case MaskVertices:
	{
		svptrarr = mActiveViewport->getSelectedVertices();
		if (svptrarr[0])
		{
			undoPush();
			setModified(true);
			for (auto svPtr : svptrarr)
			{
				svPtr->getEdges(septrarr);
				for (auto sePtr : septrarr)
				{
					DLFL::deleteEdge(&mObject, sePtr, true);
				}
				// active->recomputePatches();
				// active->recomputeNormals();
			}
		}
		mActiveViewport->clearSelectedVertices();
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
		redraw();
		break;
	}
	case MaskFaces:
	{
		sfptrarr = mActiveViewport->getSelectedFaces();
		if (sfptrarr[0])
		{
			undoPush();
			setModified(true);
			for (auto sfPtr : sfptrarr)
			{
				//get edges, collapse them
				sfPtr->getEdges(septrarr);
				for (auto sePtr : septrarr)
				{
					DLFL::collapseEdge(&mObject, sePtr, MainWindow::sDeleteEdgeCleanup);
				}
				sfPtr->getCorners(sfvptrarr);
				for (auto sfvPtr : sfvptrarr)
				{
					if (sfvPtr != nullptr)
					{
						DLFLVertexPtr vptr = sfvPtr->getVertexPtr();
						if (vptr)
						{
							vptr->getEdges(septrarr);
							for (auto sePtr : septrarr)
							{
								DLFL::deleteEdge(&mObject, sePtr, MainWindow::sDeleteEdgeCleanup);
							}
						}
					}
				} // end loop through corners of current face
			} //end loop through selected faces
			mActiveViewport->recomputePatches();
			mActiveViewport->recomputeNormals();
		}
		mActiveViewport->clearSelectedFaces();
		redraw();
		break;
	}
	case MaskCorners:
		break;
	case MaskObject://someday...
		break;
	default:
		break;
	}
}

void MainWindow::collapseSelectedEdges()
{
	DLFLEdgePtrArray septrarr = mActiveViewport->getSelectedEdges();
	if (septrarr[0])
	{
		undoPush();
		setModified(true);
		for (auto sePtr : septrarr)
		{
			if (sePtr != nullptr)
			{
				DLFL::collapseEdge(&mObject, sePtr, MainWindow::sDeleteEdgeCleanup);
				mActiveViewport->recomputeNormals();
			}
		}
		mActiveViewport->recomputePatches();
	}
	mActiveViewport->clearSelectedEdges();
	redraw();
}

void MainWindow::selectEdgesFromFaces()
{
	DLFLFacePtrArray& sfptrarr = mActiveViewport->getSelectedFaces();
	//loop through selected faces, get corresponding edges, select them
	if (!sfptrarr.empty() && sfptrarr[0])
	{
		for (auto sfPtr : sfptrarr)
		{
			DLFLEdgePtrArray septrarr;
			sfPtr->getEdges(septrarr);
			for (auto sePtr : septrarr)
			{
				if (!mActiveViewport->isSelected(sePtr))
				{
					mActiveViewport->setSelectedEdge(sNumSelEdges, sePtr);
					sNumSelEdges++;
				}
			}
		}
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	}
	setMode(MainWindow::SelectEdge);
	mActiveViewport->clearSelectedFaces();
	redraw();
}

void MainWindow::selectEdgesFromVertices()
{
	//loop through selected vertices, get corresponding edges, select them
	DLFLVertexPtrArray& svptrarr = mActiveViewport->getSelectedVertices();
	if (svptrarr.size() > 0 && svptrarr[0])
	{
		for (auto svPtr : svptrarr)
		{
			DLFLEdgePtrArray septrarr;
			svPtr->getEdges(septrarr);
			for (auto sePtr : septrarr)
			{
				if (!mActiveViewport->isSelected(sePtr))
				{
					mActiveViewport->setSelectedEdge(sNumSelEdges, sePtr);
					sNumSelEdges++;
				}
			}
		}
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	}
	setMode(MainWindow::SelectEdge);
	mActiveViewport->clearSelectedVertices();
	redraw();
}

void MainWindow::selectFacesFromEdges()
{
	DLFLFacePtr fptr1, fptr2;
	//loop through selected faces, get corresponding vertices, select them
	DLFLEdgePtrArray& septrarr = mActiveViewport->getSelectedEdges();
	if (septrarr.size() > 0 && septrarr[0])
	{
		for (auto sePtr : septrarr)
		{
			sePtr->getFacePointers(fptr1, fptr2);
			if (!mActiveViewport->isSelected(fptr1))
			{
				mActiveViewport->setSelectedFace(sNumSelFaces, fptr1);
				sNumSelFaces++;
			}
			if (!mActiveViewport->isSelected(fptr2))
			{
				mActiveViewport->setSelectedFace(sNumSelFaces, fptr2);
				sNumSelFaces++;
			}
		}
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	}
	setMode(MainWindow::SelectFace);
	mActiveViewport->clearSelectedEdges();
	redraw();
}

void MainWindow::selectFacesFromVertices()
{
	DLFLVertexPtrArray svptrarr = mActiveViewport->getSelectedVertices();
	//loop through selected faces, get corresponding edges, select them
	if (svptrarr.size() > 0 && svptrarr[0])
	{
		for (auto svPtr : svptrarr)
		{
			DLFLFacePtrArray sfptrarr;
			svPtr->getFaces(sfptrarr);
			for (auto sfPtr : sfptrarr)
			{
				if (!mActiveViewport->isSelected(sfPtr))
				{
					mActiveViewport->setSelectedFace(sNumSelFaces, sfPtr);
					sNumSelFaces++;
				}
			}
		}
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	}
	setMode(MainWindow::SelectFace);
	mActiveViewport->clearSelectedVertices();
	redraw();
}

void MainWindow::selectVerticesFromFaces()
{
	DLFLFacePtrArray& sfptrarr = mActiveViewport->getSelectedFaces();

	//loop through selected faces, get corresponding vertices, select them
	if (sfptrarr.size() > 0 && sfptrarr[0])
	{
		for (auto sfPtr : sfptrarr)
		{
			DLFLFaceVertexPtrArray sfvptrarr;
			sfPtr->getCorners(sfvptrarr);
			for (auto sfvPtr : sfvptrarr)
			{
				DLFLVertexPtr vptr = sfvPtr->getVertexPtr();
				if (!mActiveViewport->isSelected(vptr))
				{
					mActiveViewport->setSelectedVertex(sNumSelVerts, vptr);
					sNumSelVerts++;
				}
			}
		}
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	}
	setMode(MainWindow::SelectVertex);
	mActiveViewport->clearSelectedFaces();
	redraw();
}

void MainWindow::selectVerticesFromEdges()
{
	DLFLEdgePtrArray septrarr = mActiveViewport->getSelectedEdges();
	//loop through selected faces, get corresponding vertices, select them
	if (septrarr.size() > 0 && septrarr[0])
	{
		for (auto sePtr : septrarr)
		{
			DLFLVertexPtr vptr1, vptr2;
			sePtr->getVertexPointers(vptr1, vptr2);
			if (!mActiveViewport->isSelected(vptr1))
			{
				mActiveViewport->setSelectedVertex(sNumSelVerts, vptr1);
				sNumSelVerts++;
			}
			if (!mActiveViewport->isSelected(vptr2))
			{
				mActiveViewport->setSelectedVertex(sNumSelVerts, vptr2);
				sNumSelVerts++;
			}
		}
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	}
	setMode(MainWindow::SelectVertex);
	mActiveViewport->clearSelectedEdges();
	redraw();
}

void MainWindow::growSelection()
{
	switch (mSelectionMask)
	{
	case MaskVertices:
	{
		//loop through selected vertices, get corresponding edges
		DLFLVertexPtrArray& svptrarr = mActiveViewport->getSelectedVertices();
		if (svptrarr.size() > 0 && svptrarr[0])
		{
			for (auto svPtr : svptrarr)
			{
				DLFLEdgePtrArray septrarr;
				svPtr->getEdges(septrarr);
				//loop through edges, select all vertices connected to these edges
				for (auto sePtr : septrarr)
				{
					DLFLVertexPtr vp1, vp2;
					//get the two vertices for each edge, select them if they aren't already selected
					sePtr->getVertexPointers(vp1, vp2);
					if (!mActiveViewport->isSelected(vp1))
					{
						mActiveViewport->setSelectedVertex(sNumSelVerts, vp1);
						sNumSelVerts++;
					}
					if (!mActiveViewport->isSelected(vp2))
					{
						mActiveViewport->setSelectedVertex(sNumSelVerts, vp2);
						sNumSelVerts++;
					}
				}
			}
		}
		redraw();
		break;
	}
	case MaskEdges:
	{
		//loop through selected edges, get vertices, then get those edges
		DLFLEdgePtrArray& septrarr = mActiveViewport->getSelectedEdges();
		if (septrarr.size() > 0 && septrarr[0])
		{
			for (auto sePtr : septrarr)
			{
				DLFLVertexPtr vptr1, vptr2;
				DLFLEdgePtrArray eptrarray;
				//get the 2 vertices for the current edge
				sePtr->getVertexPointers(vptr1, vptr2);
				//get the edges for vertex 1
				vptr1->getEdges(eptrarray);
				for (auto v1EdgePtr : eptrarray)
				{
					if (!mActiveViewport->isSelected(v1EdgePtr))
					{
						mActiveViewport->setSelectedEdge(sNumSelEdges, v1EdgePtr);
						sNumSelEdges++;
					}
				}
				//get edges for vertex 2
				vptr2->getEdges(eptrarray);
				for (auto v2EdgePtr : eptrarray)
				{
					if (!mActiveViewport->isSelected(v2EdgePtr))
					{
						mActiveViewport->setSelectedEdge(sNumSelEdges, v2EdgePtr);
						sNumSelEdges++;
					}
				}
			}
		}
		redraw();
		break;
	}
	case MaskFaces:
	{
		DLFLFacePtrArray& sfptrarr = mActiveViewport->getSelectedFaces();
		//loop through selected faces
		if (sfptrarr.size() > 0 && sfptrarr[0])
		{
			for (auto sfPtr : sfptrarr)
			{
				//get the edges for the current face
				DLFLEdgePtrArray eptrarray;
				sfPtr->getEdges(eptrarray);
				for (auto faceEdgePtr : eptrarray)
				{
					DLFLFacePtr fptr1, fptr2;
					//get the 2 faces for the current edge,
					//select them if they aren't already selected
					faceEdgePtr->getFacePointers(fptr1, fptr2);
					if (!mActiveViewport->isSelected(fptr1))
					{
						mActiveViewport->setSelectedFace(sNumSelFaces, fptr1);
						sNumSelFaces++;
					}
					if (!mActiveViewport->isSelected(fptr2))
					{
						mActiveViewport->setSelectedFace(sNumSelFaces, fptr2);
						sNumSelFaces++;
					}
				}
			}
		}
		redraw();
		break;
	}
	case MaskCorners:
		break;
	case MaskObject:
		break;
	default:
		break;
	}
}

void MainWindow::shrinkSelection()
{
	int i = 0;

	DLFLEdgePtrArray eptrarray;

	switch (mSelectionMask)
	{
	case MaskVertices:
	{
		//loop through selected vertices, get corresponding edges
		DLFLVertexPtrArray& svptrarr = mActiveViewport->getSelectedVertices();
		if (!svptrarr.empty() && svptrarr[0])
		{
			vector<bool> deselectvertices(svptrarr.size());
			for (auto svPtr : svptrarr)
			{
				deselectvertices[i] = false;
				DLFLEdgePtrArray septrarr;
				svPtr->getEdges(septrarr);
				//loop through edges, select all vertices connected to these edges
				for (auto sePtr : septrarr)
				{
					DLFLVertexPtr vp1, vp2;
					//get the two vertices for each edge, select them if they aren't already selected
					sePtr->getVertexPointers(vp1, vp2);
					if (!mActiveViewport->isSelected(vp1) || !mActiveViewport->isSelected(vp2))
						deselectvertices[i] = true;
				}
				i++;
			}
			//loop back through verts, now deselect the flagged ones
			i = 0;
			for (auto svPtr : svptrarr)
			{
				//if it's flagged, deselect it
				if (deselectvertices[i])
					mActiveViewport->clearSelectedVertex(svPtr);
				i++;
			}
		}
		redraw();
		break;
	}
	case MaskEdges:
	{
		//loop through selected edges, get vertices, then get those edges
		DLFLEdgePtrArray& septrarr = mActiveViewport->getSelectedEdges();
		if (!septrarr.empty() && septrarr[0])
		{
			vector<bool> deselectedges(septrarr.size());
			for (auto sePtr : septrarr)
			{
				DLFLVertexPtr vptr1, vptr2;
				//get the 2 vertices for the current edge
				sePtr->getVertexPointers(vptr1, vptr2);
				//get the edges for vertex 1
				vptr1->getEdges(eptrarray);
				deselectedges[i] = false;
				for (auto v1EdgePtr : eptrarray)
				{
					if (!mActiveViewport->isSelected(v1EdgePtr))
						deselectedges[i] = true;
				}
				//get edges for vertex 2
				vptr2->getEdges(eptrarray);
				for (auto v2EdgePtr : eptrarray)
				{
					if (!mActiveViewport->isSelected(v2EdgePtr))
						deselectedges[i] = true;
				}
				i++;
			}
			//loop back through edges, now deselect the flagged ones
			i = 0;
			for (auto sePtr : septrarr)
			{
				//if it's flagged, deselect it
				if (deselectedges[i])
					mActiveViewport->clearSelectedEdge(sePtr);
				i++;
			}
		}
		redraw();
		break;
	}
	case MaskFaces:
	{
		//loop through selected faces
		DLFLFacePtrArray& sfptrarr = mActiveViewport->getSelectedFaces();
		if (!sfptrarr.empty() && sfptrarr[0])
		{
			vector<bool> deselectfaces(sfptrarr.size());
			for (auto sfPtr : sfptrarr)
			{
				//get the edges for the current face
				sfPtr->getEdges(eptrarray);
				//by default don't deselect this face
				deselectfaces[i] = false;
				for (auto faceEdgePtr : eptrarray)
				{
					DLFLFacePtr fptr1, fptr2;
					//get the 2 faces for the current edge,
					//check if both of them are selected, flag the current face
					faceEdgePtr->getFacePointers(fptr1, fptr2);
					if (!mActiveViewport->isSelected(fptr1) || !mActiveViewport->isSelected(fptr2))
						deselectfaces[i] = true;
				}
				i++;
			}
			//loop back through faces, now deselect the flagged ones
			i = 0;
			for (auto sfPtr : sfptrarr)
			{
				//if it's flagged, deselect it
				if (deselectfaces[i])
					mActiveViewport->clearSelectedFace(sfPtr);
				i++;
			}
		}
		redraw();
		break;
	}
	case MaskCorners:
		// active->selectAllCorners();
	case MaskObject:
		break;
	default:
		break;
	}
}

void MainWindow::reorderSelectedFaces()
{
	// DLFLFaceVertexPtrArray sfvptrarr;
	// vector<DLFLFaceVertexPtr>::iterator fvit;
	// DLFLFacePtrArray sfptrarr;
	// vector<DLFLFacePtr>::iterator fit;
	// //loop through selected corners, get corresponding faces, reorder them to start from the selected fvptr's
	// svptrarr = active->getSelectedVertices();
	// if ( svptrarr[0] ) {
	// 	for(vit = svptrarr.begin(); vit != svptrarr.end(); vit++){
	// 		(*vit)->getFaces(sfptrarr);
	// 		for(fit = sfptrarr.begin(); fit != sfptrarr.end(); fit++){
	// 			if (!active->isSelected(*fit)){
	// 				active->setSelectedFace(num_sel_faces,*fit);
	// 				num_sel_faces++;
	// 			}
	// 		}
	// 	}
	// 	active->recomputePatches();
	// 	active->recomputeNormals();
	// }
	// active->clearSelectedVertices();
	// redraw();
}

void MainWindow::toggleFullScreen()
{
	// go back into normal screen mode
	if (windowState() == Qt::WindowFullScreen && !mToolsToolBar->isVisible())
	{
		setWindowState(Qt::WindowNoState);
		mEditToolBar->setVisible(true);
		//mSelectionMaskToolBar->toggleViewAction();
		mPrimitivesToolBar->setVisible(true);
		mToolsToolBar->setVisible(true);
		mExtrusionToolBar->setVisible(true);
		//mConicalToolBar->toggleViewAction();
		mHighgenusToolBar->setVisible(true);
		mTexturingToolBar->setVisible(true);
		// mRemeshingToolBar->setVisible(true);
		mStatusBar->show();
	}
	// go to full screen mode 2
	else if (windowState() == Qt::WindowFullScreen)
	{
		// setWindowState(windowState() ^ Qt::WindowFullScreen);
		mStatusBar->hide();
		mEditToolBar->setVisible(false);
		mPrimitivesToolBar->setVisible(false);
		mToolsToolBar->setVisible(false);
		mExtrusionToolBar->setVisible(false);
		mHighgenusToolBar->setVisible(false);
		mTexturingToolBar->setVisible(false);
		mRemeshingToolBar->setVisible(false);
	}
	//go into full screen mode 1
	else
	{
		setWindowState(windowState() ^ Qt::WindowFullScreen);
	}
}
