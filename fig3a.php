<?php

//------------------------------------------------------------
// fichier servant à générer le graphique 3a : camembert 2D

include_once ("jpgraph/jpgraph.php");
include_once ("jpgraph/jpgraph_pie.php");

// pour dire quel format d'image on souhaite
//$graph->img->SetImgFormat('png');

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
$graph = new PieGraph(600,300,'auto');
//$graph->SetShadow();

// Set A title for the plot
$graph->title->Set("Répartition des structures appelantes");
$graph->title->SetFont(FF_FONT1,FS_BOLD);
$graph->SetColor('#CCCCCC');

// Create
$p1 = new PiePlot($data);
$p1->SetCenter(0.3,0.5);

$p1->SetLegends($title);
//$p1->SetLegends(array("Support ARI","Support La Classe","WIFI","Autres"));
//$targ=array("index.php","pie_csimex1.php#2","pie_csimex1.php#3","pie_csimex1.php#4");
//$alts=$data;
//$alts=array("val=%d","val=%d","val=%d","val=%d","val=%d","val=%d");
//$p1->SetCSIMTargets($targ,$alts);

$graph->Add($p1);

// Send back the HTML page which will call this script again
// to retrieve the image.
$graph->Stroke();
//$graph->StrokeCSIM('fig3a.php');
?>