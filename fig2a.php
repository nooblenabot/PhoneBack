<?php

//------------------------------------------------------------
// fichier servant à générer le graphique 2a : barres verticales

include ("jpgraph/jpgraph.php");
include ("jpgraph/jpgraph_bar.php");

$title = array();
$data = array();

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

// New graph with a drop shadow
$graph = new Graph(700,400,'auto');
//$graph->SetShadow();

// Use a "text" X-scale
$graph->SetScale("textlin");

// Specify X-labels
$graph->xaxis->SetTickLabels($title);

// Set title and subtitle
$graph->title->Set("Répartition des appels par tranche horaire");

// Use built in font
$graph->title->SetFont(FF_FONT1,FS_BOLD);

// Create the bar plot
$b1 = new BarPlot($data);
$b1->SetLegend("Nombre");

//$b1->SetAbsWidth(6);
//$b1->SetShadow();

// The order the plots are added determines who's ontop
$graph->Add($b1);

// Finally output the  image
$graph->Stroke();
?>