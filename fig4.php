<?php

session_start();


include ("jpgraph/jpgraph.php");
include ("jpgraph/jpgraph_line.php");

//On désérialize les 3 tableaux ici

$title = unserialize($_SESSION['title']); 
$abs = unserialize($_SESSION['abs']); 
$data = unserialize($_SESSION['data']); 

// Create the graph. These two calls are always required
$graph = new Graph(700,400,"auto");	
$graph->SetScale("textlin");
$graph->SetShadow();
$graph->img->SetMargin(70,150,50,100);	
//$graph->img->SetMargin(40,180,40,40);	
// Create the linear plots for each category

$color = array("#aa80aa", "#aaffaa", "#ffffaa", "#aaaaff","#AAECC1","#FF8000","#82B8D9","#6242C1","#C1F0E1","#FDDFDF","#7E7E7E","#F7EAA4","#A78758","#CFD7E9","#EF76BC","#64DD67");

$i = 0;
foreach ($title as $key => $value) {
  $dplot[] = new LinePlot($data[$key]);
  $dplot[$i]->SetLegend($value);
  $dplot[$i]->SetFillColor($color[$i % sizeof($color)]);
  $i++;
}

// Set xaxis labels
$graph->xaxis->SetTickLabels($abs);

// Create the accumulated graph
$accplot = new AccLinePlot($dplot);

// Add the plot to the graph
$graph->Add($accplot);

// Cette fonction permet de calculer l'intervalle d'affichage des abscisses

$graph->xaxis->SetTextTickInterval(ceil(sizeof($abs)/30));

//$graph->xaxis->title->Set("Date");  
//$graph->xaxis->title->SetFont(FF_FONT1,FS_BOLD);

//Afin de pouvoir mettre le titre des abscisses correctement je l'ai ajouté en texte directement
//et j'ai enlevé la méthode mis en commentaire ci dessus

$txt =new Text("Date");
$txt->Pos(560,310);
$txt ->SetFont(FF_FONT1,FS_BOLD);
//$txt->SetColor( "red");
$graph->AddText( $txt);


$graph->title->Set("Répartition des appels par jour et par type");
                                                                                              
$graph->yaxis->title->Set("Nombre d'appels par type");

$graph->title->SetFont(FF_FONT1,FS_BOLD);
$graph->yaxis->title->SetFont(FF_FONT1,FS_BOLD);

$graph->xaxis->SetFont(FF_VERDANA,FS_NORMAL,7);
$graph->xaxis->SetLabelAngle(50);

// Display the graph
$graph->Stroke();

?>

