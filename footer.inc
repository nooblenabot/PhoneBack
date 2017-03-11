
<!----------------------------------------------->
<!--        MENU DU BAS, ET FIN DE PAGE        -->
<!----------------------------------------------->

      <SCRIPT language="javascript">
          // fonction d'ouverture de fenêtre d'aide
          function OpenHelp(adresse)
          {
              window.open(adresse, "Aide", "toolbar=no,location=no,directories=no,status=yes,scrollbars=yes,resizable=yes,copyhistory=no,width=800,height=600,left=10,top=10");
          }
      </SCRIPT>

      <TR>
        <TD>&nbsp;</TD>
      </TR>
      <TR>
	  <?php
	    $image_de_fond = "images/baspage-fond.gif";
	    if (file_exists("images/custom/baspage-fond.gif")) $image_de_fond = "images/custom/baspage-fond.gif";
	    echo "<TD class=\"bg-baspage\" style=\"background-image:url(".$image_de_fond.");background-repeat:no-repeat;\">";
	   ?>
          <table class="BAS" cellpadding=0" cellspacing="0">
            <tr>
              <td height="100">&nbsp;</td>
            </tr>
          	<tr>
          	 <td width="91" height="50" align="left"><IMG src="images/baspage-menu-aide.gif" usemap="#aide" border="0" /></td>
	   <?php
	    $logo = "images/baspage-logo.gif";
	    if (file_exists("images/custom/baspage-logo.gif")) $logo = "images/custom/baspage-logo.gif";
	    echo "<td height=\"50\"><center><img src=\"$logo\" /></td>";
	   ?>
          	 <td width="91" height="50" align="right"><IMG src="images/baspage-menu-hdp.gif" usemap="#haut" border="0" /></td>
           </tr>
          </table>
        </TD>
          <MAP name="haut">
            <AREA shape="rect" coords="0,30,90,49" href="#haut" alt="Haut de page">
          </MAP>
          <MAP name="aide">
            <?php
              // selon la rubrique dans laquelle on se situe (SAISIE, CONSULTATION, STATISTIQUES ou ADMINISTRATION)
              // on se positionne sur l'ancre correspondante du fichier aide.html 
              // 0,125,50,150
              echo "<AREA shape=\"rect\" coords=\"0,30,45,49\" href=\"javascript:OpenHelp('aide.html#".$actions[0]."')\" alt=\"Aide\" >" ;
            ?>
          </MAP>
        </TR>
      </TABLE>
    </CENTER>
  </BODY>
</HTML>
