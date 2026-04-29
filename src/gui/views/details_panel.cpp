#include "details_panel.hpp"

#include <QtGlobal>

namespace sps::gui {

DetailsPanel::DetailsPanel(QWidget* parent)
    : QTextEdit(parent)
{
    setReadOnly(true);
    setMinimumWidth(350);
    setPlaceholderText(QString::fromUtf8("\xe2\x86\x90 행을 클릭하면 상세 정보가 표시됩니다"));
}

void DetailsPanel::showResult(const core::ScanResult& r) {
    QString html;
    html += QString("<h3>%1:%2/%3 &mdash; %4</h3>")
        .arg(QString::fromStdString(r.target_host))
        .arg(r.port)
        .arg(QString::fromStdString(r.protocol))
        .arg(QString::fromStdString(r.service.name));

    html += QString("<b>Product:</b> %1 %2<br>")
        .arg(QString::fromStdString(r.service.product))
        .arg(QString::fromStdString(r.service.version));

    if (!r.service.extra_info.empty())
        html += QString("<b>Info:</b> %1<br>")
            .arg(QString::fromStdString(r.service.extra_info));

    html += QString("<b>RTT:</b> %1 ms<br>")
        .arg(r.rtt.count());
    html += QString("<b>Risk Score:</b> %1<br><br>")
        .arg(r.max_risk(), 0, 'f', 2);

    // Banner (HTML escape for XSS prevention, 512 char truncate)
    if (!r.service.banner_raw.empty()) {
        html += "<b>Banner:</b><pre style='background:#1a1a1a; padding:8px;'>";
        html += QString::fromStdString(r.service.banner_raw)
                    .toHtmlEscaped()
                    .left(512);
        html += "</pre><br>";
    }

    if (!r.os_guess.empty())
        html += "<b>OS:</b> " + QString::fromStdString(r.os_guess).toHtmlEscaped() + "<br>";
    if (!r.cdn.empty())
        html += "<b>CDN/WAF:</b> " + QString::fromStdString(r.cdn).toHtmlEscaped() + "<br>";
    if (!r.ja4s.empty())
        html += "<b>JA4S:</b> <code>" + QString::fromStdString(r.ja4s).toHtmlEscaped() + "</code><br>";
    if (!r.ja4x.empty())
        html += "<b>JA4X:</b> <code>" + QString::fromStdString(r.ja4x).toHtmlEscaped() + "</code><br>";
    if (!r.os_guess.empty() || !r.cdn.empty() || !r.ja4s.empty() || !r.ja4x.empty())
        html += "<br>";

    // CVE list
    if (!r.cves.empty()) {
        html += "<h4>Vulnerabilities</h4>";
        for (const auto& cve : r.cves) {
            QString color = cvss_html_color(cve.cvss_score);
            QString epssColor = epss_html_color(cve.epss_score);
            const double risk = static_cast<double>(cve.cvss_score) * cve.epss_score;
            html += QString(
                "<div style='margin-bottom:8px; padding:6px; "
                "background:%1; border-radius:4px;'>"
                "<b>%2</b> &mdash; CVSS: %3 | "
                "<span style='background:%4; color:#ffffff; font-weight:bold; "
                "padding:1px 5px; border-radius:3px;'>EPSS: %5&#37;</span><br>"
                "<b>Risk Score:</b> %6<br>"
                "<small>%7</small></div>")
                .arg(color)
                .arg(QString::fromStdString(cve.cve_id))
                .arg(cve.cvss_score, 0, 'f', 1)
                .arg(epssColor)
                .arg(qRound(cve.epss_score * 100.0))
                .arg(risk, 0, 'f', 2)
                .arg(QString::fromStdString(cve.description));
        }
    } else {
        html += "<i>No known CVEs</i>";
    }

    setHtml(html);
}

QString DetailsPanel::cvss_html_color(float score) {
    if (score >= 9.0f) return "#4a1010";
    if (score >= 7.0f) return "#4a2a08";
    if (score >= 4.0f) return "#3a3508";
    return "#2a2a2a";
}

QString DetailsPanel::epss_html_color(double epss) {
    if (epss >= 0.50) return "#b41e1e";
    if (epss >= 0.10) return "#b4aa1e";
    return "#287828";
}

} // namespace sps::gui
