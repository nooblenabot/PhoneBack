<?php
//============GESTION DES STRUCTURES APPELANTES ==========

//------------------------------------------------------------------
//    Fonction qui affiche toutes les structures appelantes

function affiche_liste_structures()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $choix ;
  global $footer_msg ;
  $barre_bouton = Barre_de_boutons_BO("","","Retour");  

  $sql = "SELECT s.id, s.nom, s.defaut FROM structure s ORDER BY 2" ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  else
  {
    echo "<CENTER>" ;
    echo "<H2>Liste des structures appelantes :</H2>" ;
    echo "<A HREF=\"index.php?action=administrer.affiche_formulaire_structure\">Ajouter</A>" ;
    echo "<BR><BR>" ;
    echo "<TABLE class=\"RESULTAT\"><TR><TH>Nom</TH><TH>par d&eacute;faut<br/>dans la liste</TH><TH colspan=\"2\">Actions</TH></TR>" ;
    while (!$res->EOF)
    {
	  if ($res->fields[2] == "oui" ) { $class_defval = " class='default_value'"; }
      else {  $class_defval ="";  }
      echo "<TR>
              <TD>".$res->fields[1]."</TD><TD align=center".$class_defval.">".$res->fields[2]."</TD><TD>&nbsp;</TD>
              <TD align=center><A HREF=index.php?action=administrer.supprime_structure&id=".$res->fields[0]."&choix=".$choix."><IMG SRC=\"images/delete.gif\" border=0></A></TD>
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
//    des données de destination des appels par défaut

function affiche_formulaire_structure($p_nom_struct="", $p_defaut="")
{
  $barre_bouton = Barre_de_boutons_BO("Insérer","Effacer","Retour");  
  $menu_html1 = ListeDeChoix("SELECT distinct 'oui', 'oui' FROM structure union SELECT distinct 'non', 'non' FROM structure order by 1","p_defaut", $p_defaut) ;
  $etoile = chpobli();

  $html = <<<END

  <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      if (document.AjoutStruct.p_nom_struct.value == "" ) {alert ('Le champ "nom" est vide !'); document.AjoutStruct.p_nom_struct.focus(); return false;}
	    return true ;
    }
  </SCRIPT>


    <FORM method="GET" name="AjoutStruct" onSubmit="return verifie_formulaire()">
     <INPUT TYPE="hidden" NAME="action" VALUE="administrer.inserer_structure">
     <INPUT TYPE="hidden" NAME="choix" VALUE="4">
      <H2>Veuillez saisir ou modifier la structure appelante :</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD>nom$etoile</TD>
          <TD><INPUT type="text" name="p_nom_struct" value="$p_nom_struct"></TD>
        </TR>
        <TR>
          <TD>sélectionner cette structure par défaut<br/> dans le formulaire de prise d'appel</TD>
          <TD>$menu_html1</TD>
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
//    Fonction qui insère une nouvelle structure appelante 

function inserer_structure($p_nom,$p_defaut)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  
  // si p_defaut = Oui, alors on met tous les autres enregistrements à "Non"
  if ($p_defaut == 'oui' ) 
  {
  	$sql = "UPDATE structure set defaut = 'non'";
  	$res = $conn->Execute($sql) ;
  }
  $sql = "INSERT INTO structure ( nom, defaut ) ".
          "VALUES (".$conn->qstr($p_nom).",".$conn->qstr($p_defaut).")" ;
  $res = $conn->Execute($sql) ;

  if ($res == true)
  {
    $footer_msg = "Une structure appelante a bien &eacute;t&eacute; ins&eacute;r&eacute;e dans la base" ;
  }
  else
  {
     $footer_msg = "Il y a eu un probl&egrave;me lors de l'insertion dans la base" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}



//------------------------------------------------------------------
//    Fonction qui détruit une structure appelante

function supprime_structure()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql = "DELETE FROM structure WHERE id=".$_REQUEST["id"] ;
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