<?php

//------------------------------------------------------------
// fichier servant à générer le graphique 1a : camembert 3D

include_once ("jpgraph/jpgraph.php");
include_once ("jpgraph/jpgraph_pie.php");
include_once ("jpgraph/jpgraph_pie3d.php");

$data = array();
$title = array();

// on va chercher les valeurs passées en request,
// et on les met dans deux tableaux ($date et $title)
foreach($_REQUEST as $key => $value)
{
  if(preg_match("/^val_(\d+)$/", $key, $res))
  {
    array_push($data, $value);
    array_push($title, $_REQUEST["title_".$res[1]]);
  }
}

// Create the Pie Graph.
$graph = new PieGraph(600,200,'auto');
//$graph->SetShadow();

// Set A title for the plot
$graph->title->Set("Répartition en % des appels selon le type");
$graph->title->SetFont(FF_FONT1,FS_BOLD);
//$graph->SetMarginColor('red');
$graph->SetColor('#CCCCCC');

// Create
$p1 = new PiePlot3D($data);
$p1->SetLegends($title);
//$targ=array("pie3d_csimex1.php?v=1","pie3d_csimex1.php?v=2","pie3d_csimex1.php?v=3",
//      "pie3d_csimex1.php?v=4","pie3d_csimex1.php?v=5","pie3d_csimex1.php?v=6");
//$alts=array("val=%d","val=%d","val=%d","val=%d","val=%d","val=%d");
$p1->SetCSIMTargets($targ,$alts);

// Use absolute labels
$p1->SetLabelType(1);
$p1->value->SetFormat("%.2f");

// Move the pie slightly to the left
$p1->SetCenter(0.35,0.5);

$graph->Add($p1);

// Anti-aliasing
//$graph->img->SetAntiAliasing();

$graph->Stroke();
//$graph->StrokeCSIM('fig1a.php') ;
?>