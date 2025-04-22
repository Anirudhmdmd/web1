pipeline {
    agent any
    environment {
        // Add MinGW GCC path to system path so Jenkins can find gcc
        PATH = "C:\\MinGW\\bin;${env.PATH}" // ⚠️ Replace with actual MinGW path
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
                    // Compile with header include if needed
                    bat 'gcc -o bln_node app_bln.c -I"C:\\opt\\comptel\\eventlink\\sdk\\el-c-cxx-sdk\\include"' // ⚠️ Adjust path to nodebase.h
                    
                    // Create EL package structure
                    bat 'mkdir el_package\\bin'
                    bat 'mkdir el_package\\config'
                    
                    // Move files
                    bat 'move bln_node el_package\\bin\\'
                    bat 'move cfg.xml el_package\\config\\'
                    bat 'move dfd.xml el_package\\config\\'
                    bat 'move desc.asn el_package\\config\\'
                    
                    // Create zip package
                    bat 'powershell -Command "Compress-Archive -Path el_package\\* -DestinationPath bln_node.el"'
                }
            }
        }
        stage('Archive Package') {
            steps {
                archiveArtifacts artifacts: '**/*.el', fingerprint: true
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
