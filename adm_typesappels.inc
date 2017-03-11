<?php
//============GESTION DES TYPES D'APPELS ==========

//------------------------------------------------------------------
//    Fonction qui affiche tous les types d'appels
//  

function affiche_liste_typesappels()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  $barre_bouton = Barre_de_boutons_BO("","","Retour");  

  $sql = "SELECT id, libelle, description, defaut FROM type order by 2" ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  else
  {
    echo "<CENTER>" ;
    echo "<H2>Types d'appels :</H2>" ;
    echo "<A HREF=\"index.php?action=administrer.affiche_formulaire_typeappel\">Ajouter</A>" ;
    echo "<BR><BR>" ;
    echo "<TABLE class=\"RESULTAT\"><TR>
    		<TH>Libell&eacute;</TH>
    		<TH>Description</TH>
    		<TH>S&eacute;lection<br>par d&eacute;faut</TH>
    		<TH colspan=\"2\">Actions</TH></TR>" ;
    while (!$res->EOF)
    {
      echo "<TR><TD>".$res->fields[1]."</TD><TD>".$res->fields[2]."</TD>";
      if ($res->fields[3] == "oui" ) { $class_defval = " class='default_value'"; }
      else {  $class_defval ="";  }
      echo "<TD".$class_defval.">".$res->fields[3]."</TD>
                <TD align=\"center\"><A HREF=index.php?action=administrer.affiche_formulaire_typeappel&idtype=".$res->fields[0]."&choix=5><IMG SRC=\"images/edit.gif\" border=0></A></TD>
                <TD align=\"center\"><A HREF=index.php?action=administrer.supprime_typeappel&id=".$res->fields[0]."&choix=5><IMG SRC=\"images/delete.gif\" border=0></A></TD>
            </TR>" ;
      $res->MoveNext() ;
    }
    echo "</TABLE><br><br>" ;
	echo $barre_bouton;
    echo "</CENTER>" ;
  }
}

//------------------------------------------------------------------
//    Fonction qui affiche le formulaire de saisie
//    des type d'appels

function affiche_formulaire_typeappel($p_id="")
{
  global $conn ;
  $etoile = chpobli();
  
  if ($p_id == "" )   {
	  $lib_action = "Insérer";
  }
  else  {
	  $lib_action = "Modifier";
	  // Init des valeurs du formulaire
	  $sql = "SELECT libelle, description, defaut FROM type WHERE id = ".$p_id." order by 2" ;
	  $res = $conn->Execute($sql) ;
	  if($conn->ErrorNo() != 0)
	  {
	    return ($conn->ErrorMsg()) ;
	  }
	  $p_lib = $res->fields[0];
	  $p_desc = $res->fields[1];
	  $p_defaut = $res->fields[2];
  }
  $barre_bouton = Barre_de_boutons_BO($lib_action, "Effacer", "Retour");  
  $menu_oui_non = ListeDeChoix("SELECT distinct 'oui', 'oui' FROM type union SELECT distinct 'non', 'non' FROM type order by 1","p_defaut", $p_defaut) ;

    $html = <<<END
  
  <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      if (document.AjoutType.p_lib.value == "" ) {alert ('Le champ "libellé" est vide !'); document.AjoutType.p_lib.focus(); return false;}
      return true ;
    }
  </SCRIPT>


    <FORM method="GET" name="AjoutType" onSubmit="return verifie_formulaire()">
     <INPUT TYPE="hidden" NAME="action" VALUE="administrer.inserer_modifier_typeappel">
     <INPUT TYPE="hidden" NAME="choix" VALUE="5">
     <INPUT TYPE="hidden" NAME="idtype" VALUE="$p_id">
      <H2>Veuillez saisir ou modifier la structure appelante :</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD>Libell&eacute;$etoile</TD>
          <TD><INPUT type="text" name="p_lib" value="$p_lib"></TD>
        </TR>
        <TR>
          <TD>Description</TD>
          <TD><INPUT type="text" name="p_desc" value="$p_desc"></TD>
        </TR>
        <TR>
          <TD>sélectionner ce type d'appel<br/> par défaut dans le formulaire<br/> de prise des appels</TD>
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
//    Fonction qui insère un nouveau type d'appel 

function inserer_modifier_typeappel($p_id = "")
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  
  // si p_defaut = Oui, alors on met tous les autres enregistrements à "Non"
  if ($_REQUEST["p_defaut"] == 'oui' ) 
  {
  	$sql = "UPDATE type set defaut = 'non'";
  	$res = $conn->Execute($sql) ;
  }
  if ($p_id == "")
  {
	  $sql = "INSERT INTO type ( libelle, description, defaut ) ".
          "VALUES (".$conn->qstr($_REQUEST["p_lib"]).",".$conn->qstr($_REQUEST["p_desc"]).",".$conn->qstr($_REQUEST["p_defaut"]).")" ;
      $lib_action = "l'insertion";
      $participe_passe = "ins&eacute;r&eacute;e";
  }
  else 
  {
	  $sql = "UPDATE type set libelle = '".$_REQUEST["p_lib"]."', description = '".$_REQUEST["p_desc"]."', defaut = '".$_REQUEST["p_defaut"]."' WHERE id = ".$p_id;
      $lib_action = "la modification";
      $participe_passe = "modifi&eacute;";
  }
  $res = $conn->Execute($sql) ;

  if ($res == true)
  {
    $footer_msg = "Un type d'appel a bien &eacute;t&eacute; ".$participe_passe." dans la base" ;
  }
  else
  {
     $footer_msg = "Il y a eu un probl&egrave;me lors de ".$lib_action." dans la base" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}

//------------------------------------------------------------------
//    Fonction qui détruit un type d'appel

function supprime_typeappel()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql = "DELETE FROM type WHERE id=".$_REQUEST["id"] ;
  $res = $conn->Execute($sql) ;
  if ($res==true)
  {
    $footer_msg = "La structure appelante a bien &eacute;te d&eacute;truite de la base" ;
  }
  else
  {
    $footer_msg = "Il y a eu un probl&eagrave;me lors de la suppression : ".$conn->ErrorMsg() ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}

?>