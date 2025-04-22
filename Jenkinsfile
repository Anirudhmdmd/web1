pipeline {
    agent any
    environment {
        WORKSPACE = "${env.WORKSPACE}"
    }
    stages {
        stage('Checkout') {
            steps {
                checkout scm
            }
        }
        stage('Compile BLN Node') {
            steps {
                script {
                    // Compile the C code using gcc
                    bat 'gcc -o bln_node app_bln.c' // This will compile the C file to an executable named 'bln_node'
                    
                    // Create EL package structure (directories and move files)
                    bat 'mkdir el_package\\bin' // Create bin folder
                    bat 'mkdir el_package\\config' // Create config folder
                    bat 'move bln_node el_package\\bin\\' // Move the compiled binary to the bin folder
                    bat 'move cfg.xml el_package\\config\\' // Move cfg.xml to the config folder
                    bat 'move dfd.xml el_package\\config\\' // Move dfd.xml to the config folder
                    bat 'move desc.asn el_package\\config\\' // Move desc.asn to the config folder
                    
                    // Create a .el package (zip or other format)
                    bat 'zip -r bln_node.el el_package\\' // Create a .zip file of the entire EL package
                }
            }
        }
        stage('Archive Package') {
            steps {
                archiveArtifacts artifacts: '**/*.el', fingerprint: true // Archive the .el package
            }
        }
    }
    post {
        always {
            cleanWs() // Clean up workspace after the pipeline finishes
        }
    }
}
