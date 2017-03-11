<!----------------------------------------------->
<!--              MENU DU HAUT                 -->
<!----------------------------------------------->
<A name="haut">
<TABLE class="GL" cellpadding="0" cellspacing="0">
  <TR>
    <TD class="bg-entete" style="background-image:url(<?php echo get_my_image("entete-fond.gif"); ?>);background-repeat:no-repeat;">
      <IMG src="images/entete-menu.gif" usemap="#menu" border="0"></TD>
    </TD>
    <MAP name="menu">
<?php
  if (a_le_droit("SAISIE") == true)  echo "<AREA shape=\"rect\" coords=\"41,72,191,91\" href=\"index.php?action=saisir\" alt=\"Saisie\">"; 
  if (a_le_droit("CONSULTATION") == true)  echo "<AREA shape=\"rect\" coords=\"207,117,357,137\" href=\"index.php?action=consulter\" alt=\"Consultation\">"; 
  if (a_le_droit("STATISTIQUES") == true)  echo "<AREA shape=\"rect\" coords=\"370,92,520,111\" href=\"index.php?action=states\" alt=\"Statistiques\">"; 
  if (a_le_droit("ADMINISTRATION") == true)  echo "<AREA shape=\"rect\" coords=\"543,113,693,132\" href=\"index.php?action=administrer\" alt=\"Administration\">";  
?>
    </MAP>
  </TR>

  <TR>
    <TD>
      <BR>
<!----------------------------------------------->
