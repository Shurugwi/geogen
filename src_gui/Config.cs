﻿/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

using System;
using System.ComponentModel;

namespace GeoGen_Studio
{
    [System.Xml.Serialization.XmlInclude(typeof(Main.ActionAfterExectution))]
    [System.Xml.Serialization.XmlInclude(typeof(Main.ModelDetailLevel))]
    [System.Xml.Serialization.XmlInclude(typeof(Main.ViewportBackground))]
    [System.Xml.Serialization.XmlInclude(typeof(OpenTK.Vector4))]
    public class Config
    {

        public string templateFile;
        public bool openLastFileOnStartup;
        public string lastFile;
        public string scintillaDefinitonsFile;
        public Main.ActionAfterExectution actionAfterExec;
        public string geoGenPath;
        public string mainMapOutputFile;
        public string scriptTempFile;
        public string geoGenWorkingDirectory;
        public string overlayDirectory;
        public string documentationPath;
        public int heightScale;
        public string lastImportedFile;
        public string lastExportedFile;
        public bool exportRescaleMode;
        public string lastImportedTexture;
        public string lastExportedOutput;
        public string lastExportedScreenshot;
        public uint[] parameters;
        public uint seed;

		/* Window layout settings */
        public int mainSplitter;
        public int sidebarSplitter;
        public Main.SidebarMode sidebarMode;
        public bool showStatusbar;
        public bool showSidebar;
        public bool showToolbar;
        public bool showConsole;
        public bool showParameters;
        public bool wordWrap;
        public bool lineBreaks;
        public bool whitespace;
        public int editorZooom;

        public Main.BitmapDetailLevel mapDetailLevel;

        private Main.ModelDetailLevel modelDetailLevel;
        private Main.BitmapDetailLevel textureDetailLevel;
        public bool wireframe;
        public OpenTK.Vector4 ambientLightColor;
        public OpenTK.Vector4 directionalLightColor;
        public float lightAzimuth;
        public float lightElevation;
        public bool lightEnabled;
        public bool enableTerrainUnderZero;
        public bool enable3d;
        public int defaultTextureOverlay;
        public Main.ViewportBackground backgroundColor3d;

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to the template file used when creating new file."), DefaultValue("./../examples/template.nut")]
        public string TemplateFile
        {
            get { return templateFile; }
            set { templateFile = value; }
        }

        [CategoryAttribute("Startup"), DescriptionAttribute("Should the last opened file be opened when starting the application?"), DefaultValue(true)]
        public bool OpenLastFileOnStartup
        {
            get { return openLastFileOnStartup; }
            set { openLastFileOnStartup = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to the XML file with syntax highlighting and autocompletion definitions."), DefaultValue("../config/scintilla.xml")]
        public string ScintillaDefinitonsFile
        {
            get { return scintillaDefinitonsFile; }
            set { scintillaDefinitonsFile = value; }
        }

        [CategoryAttribute("Script Execution"), DescriptionAttribute("What should the program do once script execution is finished?"), DefaultValue(Main.ActionAfterExectution.GoTo3DOutput)]
        public Main.ActionAfterExectution ActionAfterExec
        {
            get { return actionAfterExec; }
            set { actionAfterExec = value; }
        }

        [CategoryAttribute("Script Execution"), DescriptionAttribute("Maximum map size accepted by GeoGen Studio. All maps will be downcaled to match this size. Decrease this value if the application is frequently running out of memory."), DefaultValue(Main.BitmapDetailLevel.VeryHigh_2048x2048Pixels)]
        public Main.BitmapDetailLevel MapDetailLevel
        {
            get { return mapDetailLevel; }
            set { mapDetailLevel = value; }
        }

        [CategoryAttribute("Script Execution"), DescriptionAttribute("Random seed used to generate the terrain. Two terrains generated with the same seed, script and GeoGen version are guaranteed to be always exactly same. Use value '0' to create a new seed automatically every time script is executed."), DefaultValue(0)]
        public uint RandomSeed
        {
            get { return seed; }
            set { seed = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to a compatible GeoGen executable."), DefaultValue("../bin/geogen.exe")]
        public string GeoGenPath
        {
            get { return geoGenPath; }
            set { geoGenPath = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Base directory for all GeoGen temporary files."), DefaultValue("../temp/studio_maps/temp")]
        public string GeoGenWorkingDirectory
        {
            get { return geoGenWorkingDirectory; }
            set { geoGenWorkingDirectory = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path where temporary script will be saved, relative to GeoGenWorkingDirectory."), DefaultValue("../../studio_temp.nut")]
        public string ScriptTempFile
        {
            get { return scriptTempFile; }
            set { scriptTempFile = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path where main output map will be saved, relative to GeoGenWorkingDirectory."), DefaultValue("../main.shd")]
        public string MainMapOutputFile
        {
            get { return mainMapOutputFile; }
            set { mainMapOutputFile = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Directory where overlays are stored."), DefaultValue("../overlays")]
        public string OverlayDirectory
        {
            get { return overlayDirectory; }
            set { overlayDirectory = value; }
        }

        [CategoryAttribute("Paths"), DescriptionAttribute("Path to main documentation file."), DefaultValue("../documentation.html")]
        public string DocumentationPath
        {
            get { return documentationPath; }
            set { documentationPath = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Highest allowed amount of polygons for the terrain model in the 3D view. Higher settings require more powerful graphics card and the models take longer to build. Has major impact on performance."), DefaultValue(Main.ModelDetailLevel.Medium_512x512Polygons)]
        public Main.ModelDetailLevel ModelDetailLevel
        {
            get { return modelDetailLevel; }
            set { modelDetailLevel = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Highest allowed resolution for the terrain texture in the 3D view. Higher settings require more powerful graphics card. Has medium impact on performance."), DefaultValue(Main.BitmapDetailLevel.VeryHigh_2048x2048Pixels)]
        public Main.BitmapDetailLevel TextureDetailLevel
        {
            get { return textureDetailLevel; }
            set { textureDetailLevel = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Should the underwater terrain be modelled in 3D? Change of this setting will take effect once the model is rebuilt."), DefaultValue(false)]
        public bool EnableTerrainUnderZero
        {
            get { return enableTerrainUnderZero; }
            set { enableTerrainUnderZero = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Enable/disable the 3D view."), DefaultValue(true)]
        public bool Enable3D
        {
            get { return enable3d; }
            set { enable3d = value; }
        }

        [CategoryAttribute("3D View"), DescriptionAttribute("Color covering the unpainted areas of the 3D viewport."), DefaultValue(Main.ViewportBackground.Black)]
        public Main.ViewportBackground BackgroundColor3d
        {
            get { return backgroundColor3d; }
            set { backgroundColor3d = value; }
        }

        public Config()
	    {
            this.LoadDefaults();
	    }

        private void Serialize(string file, Config config)
        {
            // make sure the config directory exists
            if (!System.IO.Directory.Exists("../config"))
            {
                System.IO.Directory.CreateDirectory("../config");
            }

            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(Config));

            System.IO.StreamWriter writer = System.IO.File.CreateText(file);
            
            xs.Serialize(writer, config);

            writer.Flush();
            writer.Close();
        }

        private static Config Deserialize(string file)
        {
            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(Config));

            System.IO.StreamReader reader = System.IO.File.OpenText(file);

            try
            {
                Config c = (Config)xs.Deserialize(reader);
                return c;
            }
            // make sure the config file is closed so it can be recreated in case of failure
            finally
            {
                reader.Close();
            }
        }

        private void LoadDefaults() 
        { 
            /* Basic settings */
            templateFile = "./../examples/template.nut";
            openLastFileOnStartup = true;
            lastFile = "";
            scintillaDefinitonsFile = "../config/scintilla.xml";
            lastImportedFile = "";
            lastExportedFile = "";
            lastImportedTexture = "";
            lastExportedScreenshot = "";
            lastExportedOutput = "";
            parameters = new uint[0];
            
            exportRescaleMode = false;
            mapDetailLevel = Main.BitmapDetailLevel.VeryHigh_2048x2048Pixels;

            /* Execution settings */
            actionAfterExec = Main.ActionAfterExectution.GoTo3DOutput;
            geoGenPath = "../bin/geogen.exe";
            geoGenWorkingDirectory = "../temp/studio_maps/temp";
            scriptTempFile = "../../studio_temp.nut";
            mainMapOutputFile = "../main.shd";

            /* Output Viewer settings */
            overlayDirectory = "../overlays";

            /* Documentation Viewer settings */
            documentationPath = "../documentation.html";

            /* 3D view */
            modelDetailLevel = Main.ModelDetailLevel.Medium_512x512Polygons;
            textureDetailLevel = Main.BitmapDetailLevel.VeryHigh_2048x2048Pixels;
            enableTerrainUnderZero = false;
            enable3d = true;
            defaultTextureOverlay = 7;

            /* 3D light */
            ambientLightColor = new OpenTK.Vector4(0.2f, 0.2f, 0.2f, 1.0f);
            OpenTK.Vector4 directionalLightColor = new OpenTK.Vector4(0.6f, 0.6f, 0.6f, 1.0f);
            lightAzimuth = 0.0f;
            lightElevation = 0.5f;
            lightEnabled = true;
            backgroundColor3d = Main.ViewportBackground.Black;
        }

        public void Save()
        {
            Main.Get().SaveInterfaceSettings();
            Serialize("../config/studio.xml", this);
        }

        public static void Load()
        {
            Main main = Main.Get();
            
            // try to load the config
            try
            {
                Config c = Deserialize("../config/studio.xml");

                main.config = c;

                main.LoadInterfaceSettings();
            }
            // something went wrong -> create a new config file
            catch (System.Exception e)
            {
                main.WriteToConsole("Could not open config file, falling back to defaults.");

                Config c = new Config();

                main.config = c;

                // save the newly created cofig
                c.Serialize("../config/studio.xml", c);
            }
        }
    }
}
