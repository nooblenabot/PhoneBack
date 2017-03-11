<?php
//==================================================================
// GESTION DES DROITS D'ACCES.
//==================================================================

//------------------------------------------------------------------
//    Fonction qui supprimer tous les droits d'un utilisateur.

function supprimer_droits ($p_idutilisateur)
{
  global $conn ;
  
  // par défaut, on donne tous les droits sauf admin.
  $sql0 = "DELETE from droits_acces WHERE idutilisateur = ".$p_idutilisateur;
  $res0 = $conn->Execute($sql0) ;
}

//------------------------------------------------------------------
//    Fonction qui initialise les droits d'un utilisateur sur les 
// 	  4 rubriques, à sa création ou modification

function init_droits ($p_idutilisateur)
{
  global $conn ;

  // suppression de tout ce qui existe comme droits
  supprimer_droits ($p_idutilisateur);  
  // par défaut, on donne tous les droits sauf admin.
  $sql1 = "insert into droits_acces (idrubrique, idutilisateur, acces) select id,".$p_idutilisateur.",'oui' from rubrique WHERE lib != 'ADMINISTRATION'" ;
  $sql2 = "insert into droits_acces (idrubrique, idutilisateur, acces) select id,".$p_idutilisateur.",'non' from rubrique WHERE lib = 'ADMINISTRATION'" ;  
  $res1 = $conn->Execute($sql1) ;
  $res2 = $conn->Execute($sql2) ;

  if($conn->ErrorNo() != 0 )
  {
    return ($conn->ErrorMsg()) ;
  }
}

//------------------------------------------------------------------
//    Fonction qui modifie un droit d'un utilisateur

function compte_droits ($p_idutilisateur)
{
   global $conn ;
   $sql = "SELECT count(*) FROM droits_acces WHERE idutilisateur = ".$p_idutilisateur ;
   $res = $conn->Execute($sql) ;
   if($conn->ErrorNo() != 0)
   {
     return ($conn->ErrorMsg()) ;
   }
   
	return $res->fields[0];
}

//------------------------------------------------------------------
//    Fonction qui modifie un droit d'un utilisateur

function modifie_droit ($iddroit, $anciendroit)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  
  // si anciendroit = Oui, le nouveau est "Non" et inversement, c'est une bascule.
  if ($anciendroit == 'oui' ) {	$nouveaudroit = 'non'; } else {$nouveaudroit = 'oui'; }
  
  if ($iddroit != "")
  {
	  $sql = "UPDATE droits_acces set acces = '".$nouveaudroit."' WHERE id = ".$iddroit;
      $res = $conn->Execute($sql) ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}
//------------------------------------------------------------------
//    Fonction qui affiche la matrice des droits d'accès
//  

function affiche_matrice_droits()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  
  $barre_bouton = Barre_de_boutons_BO("","","Retour");  
  
  $sql = "SELECT lib FROM rubrique order by ordre_affichage" ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  else
  {
    echo "<CENTER>" ;
    echo "<H2>Gérer les droits des utilisateurs :</H2>" ;
    echo "<BR><BR>" ;
    // Autant de colonnes que de rubriques
    echo "<TABLE class=\"RESULTAT\"><TR>";
    echo "<TH class=\"underlined\">Utilisateur</TH>";
    while (!$res->EOF)
    {
      echo "<TH class=\"underlined\">".$res->fields[0]."</TH>";
      $res->MoveNext() ;
    }
    echo "</TR>";
        
    // autant de lignes que d'utilisateurs
    $sql1 = "SELECT id, nom, prenom FROM utilisateur order by nom" ;
    $cursor_usrs = $conn->Execute($sql1) ;
    
    while (!$cursor_usrs->EOF)
    {
	    $nb_de_droits = compte_droits($cursor_usrs->fields[0]);
	    // si pas de droits dans latable, on les crée
	    if ($nb_de_droits == 0) 
	    {
		    init_droits ($cursor_usrs->fields[0]);
	    }
	    // boucle d'affichage des droits pour un utilisateur
    	$sql2 = "SELECT d.id, d.idrubrique, d.acces FROM droits_acces d, rubrique r WHERE r.id = d.idrubrique AND d.idutilisateur = ".$cursor_usrs->fields[0]." order by r.ordre_affichage" ;
    	$droits = $conn->Execute($sql2) ;
		echo "<TR>";
		echo "<TD>".$cursor_usrs->fields[1]." ".$cursor_usrs->fields[2]."</TD>";
    	while (!$droits->EOF)
    	{
	        if ($droits->fields[2] == "oui" ) { $class_defval = " class='default_value'"; }
	        else {  $class_defval =" class='centered_value'";  }
	        
	        // pas de modif des droits possibles pour l'utilisateur connecté.
	        if ($_SESSION["id_utilisateur"] != $cursor_usrs->fields[0]) 
	        	echo "<TD".$class_defval."><a href=\"index.php?action=administrer.modifie_droit&iddroit=".$droits->fields[0]."&anciendroit=".$droits->fields[2]."&choix=6\"".$class_defval.">".$droits->fields[2]."</a></TD>";
	        else 
	        	echo "<TD".$class_defval.">".$droits->fields[2]."</TD>";
	      	$droits->MoveNext() ;
        }
	    echo "</TR>";
      $cursor_usrs->MoveNext() ;
    }

    echo "</TABLE><br><br>" ;
	echo $barre_bouton;
    echo "</CENTER>" ;
  }
}
?>