<?php

//------------------------------------------------------------
// fichier servant à générer le graphique 3b : barres horizontales

include ("jpgraph/jpgraph.php");
include ("jpgraph/jpgraph_bar.php");

$title = array();
$data = array();

// on va chercher les valeurs passées en request,
// et on les met dans deux tableaux ($date et $title)
foreach($_REQUEST as $key => $value)
{//echo "Clé: $key; Valeur: $value<br>\n";
  if(preg_match("/^val_(\d+)$/", $key, $res))
  {
    array_push($data, $value);
    array_push($title, $_REQUEST["title_".$res[1]]);
  }
}

// New graph with a drop shadow
$graph = new Graph(600,400,'auto');
// Use a "text" X-scale
$graph->SetScale("textlin");

$top = 80;
$bottom = 30;
$left = 200;
$right = 50;
$graph->Set90AndMargin($left,$right,$top,$bottom);

// Setup title
$graph->title->Set("Répartition des structures appelantes");
$graph->title->SetFont(FF_VERDANA,FS_BOLD,9);
$graph->subtitle->Set("Nombre par type de structure");

// Setup X-axis
// Specify X-labels
$graph->xaxis->SetTickLabels($title);
// ATTENTION A LA POLICE DE CARACTERES, AINSI QUE SA TAILLE
// VISIBLEMENT CELA PEUT POSER DES PROBLEMES LORS DE LA GENERATION DE PDF !
$graph->xaxis->SetFont(FF_FONT1,FS_NORMAL,9);
// Some extra margin looks nicer
$graph->xaxis->SetLabelMargin(5);
// Label align for X-axis
$graph->xaxis->SetLabelAlign('right','center');

// Create the bar plot
$b1 = new BarPlot($data);
$b1->SetFillColor('#CC99CC');
$b1->SetLegend("Nombre");

// The order the plots are added determines who's ontop
$graph->Add($b1);

// Finally output the  image
$graph->Stroke();
?>

