<?php
//*************************************************************
//                 Gestion des issues des appels 
//*************************************************************

function affiche_liste_issuesappels()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  $barre_bouton = Barre_de_boutons_BO("","","Retour");  

  $sql = "SELECT id, libelle, description, defaut FROM issue ORDER BY 2" ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  else
  {
    echo "<CENTER>" ;
    echo "<H2>Issues des appels :</H2>" ;
    echo "<A HREF=\"index.php?action=administrer.affiche_formulaire_issueappel\">Ajouter</A>" ;
    echo "<BR><BR>" ;
    echo "<TABLE class=\"RESULTAT\"><TR><TH>Libellé</TH><TH>Description</TH><TH>Par défaut<br/>dans la liste</TH><TH colspan=\"2\">Actions</TH></TR>" ;
    while (!$res->EOF)
    {
      echo "<TR><TD>".$res->fields[1]."</TD><TD>".$res->fields[2]."</TD>";
              
      if ($res->fields[3] == "oui" ) { $class_defval = " class='default_value'"; }
      else {  $class_defval ="";  }
      echo "<TD".$class_defval.">".$res->fields[3]."</TD>";
      
      echo "<TD align=\"center\"><A HREF=index.php?action=administrer.affiche_formulaire_issueappel&idissue=".$res->fields[0]."&choix=9><IMG SRC=\"images/edit.gif\" border=0></A></TD>";
      echo "<TD align=\"center\"><A HREF=index.php?action=administrer.supprime_issueappel&idissue=".$res->fields[0]."&choix=9><IMG SRC=\"images/delete.gif\" border=0></A></TD></TR>" ;
      $res->MoveNext() ;
    }
    echo "</TABLE><br><br>" ;    
	echo $barre_bouton;
    echo "</CENTER>" ;
  }

}

//------------------------------------------------------------------
//    Fonction qui affiche le formulaire de saisie
//    des données des issues des appels

function affiche_formulaire_issueappel($idissue="")
{
  global $conn ;
  $etoile = chpobli();

  if ($idissue == "" )   {
	  $lib_action = "Insérer";
  }
  else  {
	  $lib_action = "Modifier";
	  // Init des valeurs du formulaire
	  $sql = "SELECT id, libelle, description, defaut FROM issue WHERE id = ".$idissue ;
	  $res = $conn->Execute($sql) ;
	  if($conn->ErrorNo() != 0)
	  {
	    return ($conn->ErrorMsg()) ;
	  }
	  $idissue = $res->fields[0];
	  $p_lib = $res->fields[1];
	  $p_desc = $res->fields[2];
	  $p_defaut = $res->fields[3];
  }
  $barre_bouton = Barre_de_boutons_BO($lib_action, "Effacer", "Retour");  
  $menu_oui_non = ListeDeChoix("SELECT distinct 'oui', 'oui' FROM type union SELECT distinct 'non', 'non' FROM type order by 1","p_defaut", $p_defaut) ;

  $html = <<<END

  <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      if (document.infoIssue.p_lib.value == "" ) {alert ('Le champ "Libellé" est vide !'); document.infoIssue.p_lib.focus(); return false;}
      return true ;
    }
  </SCRIPT>


    <FORM method="GET" name="infoIssue" onSubmit="return verifie_formulaire()">
     <INPUT TYPE="hidden" NAME="action" VALUE="administrer.insere_modifie_issueappel">
     <INPUT TYPE="hidden" NAME="choix" VALUE="9">
     <INPUT TYPE="hidden" NAME="idissue" VALUE="$idissue">
      <H2>Veuillez saisir ou modifier l'issue</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD>libelle</TD>
          <TD><INPUT type="text" name="p_lib" value="$p_lib"></TD>
        </TR>
        <TR>
          <TD>Description</TD>
          <TD><INPUT type="text" name="p_desc" value="$p_desc"></TD>
        </TR>
        <TR>
          <TD>Defaut</TD>
          <TD>$menu_oui_non</TD>
        </TR>
      </TABLE>
      <BR>
      <CENTER>
		$barre_bouton
      </CENTER>
    </FORM>
END;

return(array($html, null)) ;
}


//------------------------------------------------------------------
//    Fonction qui insère une nouvelle issue d'appel

function insere_modifie_issueappel($p_id = "")
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  
  // si p_defaut = Oui, alors on met tous les autres enregistrements à "Non"
  if ($_REQUEST["p_defaut"] == 'oui' ) 
  {
  	$sql = "UPDATE issue set defaut = 'non'";
  	$res = $conn->Execute($sql) ;
  }
  if ($p_id == "")
  {
	  $sql = "INSERT INTO issue ( libelle, description, defaut ) ".
          "VALUES (".$conn->qstr($_REQUEST["p_lib"]).",".$conn->qstr($_REQUEST["p_desc"]).",".$conn->qstr($_REQUEST["p_defaut"]).")" ;
      $lib_action = "l'insertion";
      $participe_passe = "ins&eacute;r&eacute;e";
  }
  else 
  {
	  $sql = "UPDATE issue set libelle = '".$_REQUEST["p_lib"]."', description = '".$_REQUEST["p_desc"]."', defaut = '".$_REQUEST["p_defaut"]."' WHERE id = ".$p_id;
      $lib_action = "la modification";
      $participe_passe = "modifi&eacute;";
  }
  $res = $conn->Execute($sql) ;

  if ($res == true)
  {
    $footer_msg = "Une issue d'appel a bien &eacute;t&eacute; ".$participe_passe." dans la base" ;
  }
  else
  {
     $footer_msg = "Il y a eu un probl&egrave;me lors de ".$lib_action." dans la base" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}


//------------------------------------------------------------------
//    Fonction qui détruit une issue d'appel

function supprime_issueappel()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql = "DELETE FROM issue WHERE id=".$_REQUEST["idissue"] ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  if ($res==true)
  {
    $footer_msg = "L'issue de l'appel a bien &eacute;te d&eacute;truite de la base" ;
  }
  else
  {
    $footer_msg = "Il y a eu un probl&eagrave;me lors de la suppression." ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}


?>