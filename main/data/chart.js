let chartT = new Highcharts.Chart({
    chart: {
        renderTo: 'chart-ecg'
    },
    title: {
        text: 'ECG'
    },
    series: [{
        showInLegend: false,
        data: []
    }],
    plotOptions: {
        line: { 
            animation: false,
            dataLabels: {
                enabled: true
            }
        },
        series: {
            color: '#059e8a'
        }
    },
    xAxis: { 
        type: 'datetime',
        dateTimeLabelFormats: {
            second: '%H:%M:%S'
        }
    },
    yAxis: {
        title: {
            text: 'ECG'
        }
    },
    credits: {
        enabled: false
    }
});

setInterval(function() {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            let x = (new Date()).getTime(),
                y = parseFloat(this.responseText); //console.log(this.responseText);

            if (chartT.series[0].data.length > 3000) {
                chartT.series[0].addPoint([x, y], true, true, true);
            } else {
                chartT.series[0].addPoint([x, y], true, false, true);
            }
        }
    };
    xhttp.open("GET", "/ECG", true);
    xhttp.send();
}, 2);